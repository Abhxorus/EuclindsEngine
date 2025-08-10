#pragma once
#include <vector>
#include <limits>
#include <random>
#include <algorithm>
#include <cmath>
#include "Prerequisitos.h"
#include "ECS/A_Racer.h"

/*
 * WaypointFollowConfig
 * Racers will be assigned targets from the provided waypoint list.
 * Humanization knobs reduce robotic motion.
 */
struct
    WaypointFollowConfig {
    std::vector<EngineUtilities::TSharedPointer<A_Racer>> racers;
    const std::vector<sf::Vector2f>* waypoints = nullptr;
    float arriveRadiusForAdvance = 12.f;

    /* humanize */
    float reactionDelay = 0.18f;        /* seconds to wait before reacting to a new index */
    float waypointNoiseRadius = 8.f;    /* random offset added to target (pixels) */
    int   lookAhead = 1;                /* 0..2 extra waypoints to look ahead */
    float mistakeProb = 0.05f;          /* chance to briefly pick neighbor target */
    float mistakeCooldown = 2.0f;       /* seconds between mistakes */

    /* keep on track */
    float corridorWidth = 80.f;         /* max lateral distance to segment to allow advance */

    /* corner speed limiter (applies to NPCs via r->setSpeed during update) */
    bool  cornerSlowdownEnabled = true;
    float cornerMinAngleDeg = 25.f; /* below this, no slowdown */
    float cornerMaxAngleDeg = 95.f; /* at/above this, apply full slowdown */
    float cornerMinFactor = 0.82f;/* 0.82 = 82% of base speed on very sharp corners */
};

class
    WaypointFollowSystem {
public:

    WaypointFollowSystem(const WaypointFollowConfig& cfg)
        : cfg(cfg),
        indices(cfg.racers.size(), 0),
        reactTimers(cfg.racers.size(), 0.f),
        mistakeTimers(cfg.racers.size(), 0.f),
        rng(std::random_device{}()),
        uni(-1.f, 1.f),
        uni01(0.f, 1.f) {
    }

    ~WaypointFollowSystem() = default;

    /*
     * Prime indices from current positions so racers start aiming the nearest point.
     * Avoids the “teleport” look when starting the race.
     */
    void
        primeFromPositions() {
        if (!cfg.waypoints || cfg.waypoints->empty()) return;
        const auto& W = *cfg.waypoints;
        for (size_t i = 0; i < cfg.racers.size(); ++i) {
            auto& r = cfg.racers[i];
            if (!r) continue;
            EngineMathLib::CVector2 p = r->getPosition();
            float best = std::numeric_limits<float>::max();
            size_t bestIdx = 0;
            for (size_t k = 0; k < W.size(); ++k) {
                EngineMathLib::CVector2 wk{ W[k].x, W[k].y };
                float d = (wk - p).length();
                if (d < best) { best = d; bestIdx = k; }
            }
            indices[i] = bestIdx;
            reactTimers[i] = 0.f;
            mistakeTimers[i] = 0.f;
            r->setTarget(EngineMathLib::CVector2(W[bestIdx].x, W[bestIdx].y));
        }
    }

    /*
     * Update once per frame.
     * Advance only if close enough OR we actually crossed the segment
     * inside a corridor around it. Prevents skipping waypoints and keeps the racer on track.
     * Also applies corner speed limiting so the NPC does not rocket out of tight turns.
     */
    void
        update(float dt) {
        if (!cfg.waypoints || cfg.waypoints->empty()) return;
        const auto& W = *cfg.waypoints;
        if (W.empty()) return;

        for (size_t i = 0; i < cfg.racers.size(); ++i) {
            auto& r = cfg.racers[i];
            if (!r) continue;

            /* human-like reaction */
            reactTimers[i] -= dt;
            if (reactTimers[i] > 0.f) continue;

            /* base indices */
            size_t idx = indices[i];
            size_t nextIdx = (idx + 1) % W.size();
            size_t nextNextIdx = (nextIdx + 1) % W.size();

            EngineMathLib::CVector2 pos = r->getPosition();
            EngineMathLib::CVector2 wp{ W[idx].x,     W[idx].y };
            EngineMathLib::CVector2 wpN{ W[nextIdx].x, W[nextIdx].y };
            EngineMathLib::CVector2 wpNN{ W[nextNextIdx].x, W[nextNextIdx].y };

            EngineMathLib::CVector2 seg = wpN - wp;
            float segLen2 = seg.x * seg.x + seg.y * seg.y;
            if (segLen2 <= 1e-4f) segLen2 = 1.f;

            /* steering target = lookAhead (+ noise), but ADVANCE is always +1 only */
            int la = std::max(0, std::min(cfg.lookAhead, 2));
            size_t tgtIdx = (idx + static_cast<size_t>(la)) % W.size();
            EngineMathLib::CVector2 target{ W[tgtIdx].x, W[tgtIdx].y };

            /* occasional small mistake on target only (does NOT change index) */
            mistakeTimers[i] -= dt;
            if (mistakeTimers[i] <= 0.f && uni01(rng) < cfg.mistakeProb) {
                int sign = (uni01(rng) < 0.5f) ? -1 : 1;
                size_t wrong = (tgtIdx + W.size() + static_cast<size_t>(sign)) % W.size();
                target = EngineMathLib::CVector2(W[wrong].x, W[wrong].y);
                mistakeTimers[i] = cfg.mistakeCooldown;
            }

            /* add small noise to target */
            if (cfg.waypointNoiseRadius > 0.f) {
                target.x += uni(rng) * cfg.waypointNoiseRadius;
                target.y += uni(rng) * cfg.waypointNoiseRadius;
            }

            /* distance checks */
            float d_to_wp = (wp - pos).length();
            float d_to_target = (target - pos).length();

            /* projection of pos onto segment [wp, wpN] -> t in [0,1] ideally */
            EngineMathLib::CVector2 rel = pos - wp;
            float t = (rel.x * seg.x + rel.y * seg.y) / segLen2;
            float tClamped = std::max(0.f, std::min(1.f, t));

            /* perpendicular distance to segment (corridor control) */
            EngineMathLib::CVector2 proj = EngineMathLib::CVector2(wp.x + seg.x * tClamped,
                wp.y + seg.y * tClamped);
            float distToLine = (proj - pos).length();

            /* advance rules:
               1) near base waypoint, or
               2) near actual target, or
               3) crossed forward enough (t > 0.6) AND inside corridor
            */
            bool crossedForward = (t > 0.6f) && (distToLine <= cfg.corridorWidth);
            bool shouldAdvance =
                (d_to_wp < cfg.arriveRadiusForAdvance) ||
                (d_to_target < (cfg.arriveRadiusForAdvance * 0.7f)) ||
                crossedForward;

            if (shouldAdvance) {
                indices[i] = (idx + 1) % W.size();
                reactTimers[i] = cfg.reactionDelay;

                /* refresh target after advancing to avoid hesitation */
                size_t nidx = indices[i];
                size_t ntgt = (nidx + static_cast<size_t>(la)) % W.size();
                EngineMathLib::CVector2 ntarget{ W[ntgt].x, W[ntgt].y };
                if (cfg.waypointNoiseRadius > 0.f) {
                    ntarget.x += uni(rng) * cfg.waypointNoiseRadius;
                    ntarget.y += uni(rng) * cfg.waypointNoiseRadius;
                }
                if (r->getMode() != SteeringMode::Pursuit) {
                    r->setTarget(ntarget);
                }
                /* fall through to corner speed limiter below */
            }
            else {
                /* keep steering current target */
                if (r->getMode() != SteeringMode::Pursuit) {
                    r->setTarget(target);
                }
            }

            /* corner speed limiter: slow down on sharp turns to match human cornering */
            if (cfg.cornerSlowdownEnabled) {
                auto normalize = [](EngineMathLib::CVector2 v) {
                    float L = v.length();
                    if (L <= 1e-5f) return v;
                    return EngineMathLib::CVector2(v.x / L, v.y / L);
                    };
                EngineMathLib::CVector2 v1 = normalize(wpN - wp);
                EngineMathLib::CVector2 v2 = normalize(wpNN - wpN);
                float dot = v1.x * v2.x + v1.y * v2.y;
                dot = std::max(-1.f, std::min(1.f, dot));
                float angleRad = std::acos(dot);
                float angleDeg = angleRad * 57.2957795f; /* rad->deg */

                float f = 1.f;
                if (angleDeg >= cfg.cornerMaxAngleDeg) {
                    f = cfg.cornerMinFactor;
                }
                else if (angleDeg <= cfg.cornerMinAngleDeg) {
                    f = 1.f;
                }
                else {
                    /* linear interpolation between 1.0 and cornerMinFactor */
                    float tspan = (angleDeg - cfg.cornerMinAngleDeg) /
                        std::max(1.f, (cfg.cornerMaxAngleDeg - cfg.cornerMinAngleDeg));
                    f = 1.f + (cfg.cornerMinFactor - 1.f) * tspan;
                }

                /* apply only a reduction (never boost) and only for this frame
                   assumes BaseApp enforces base speed before calling update() */
                float baseSpd = r->getSpeed();     /* current enforced by BaseApp */
                float limited = baseSpd * std::min(1.f, std::max(cfg.cornerMinFactor, f));
                if (limited < baseSpd) {
                    r->setSpeed(limited);
                }
            }
        }
    }

private:
    WaypointFollowConfig cfg;
    std::vector<size_t> indices;
    std::vector<float>  reactTimers;
    std::vector<float>  mistakeTimers;

    std::mt19937 rng;
    std::uniform_real_distribution<float> uni;   /* [-1, 1] */
    std::uniform_real_distribution<float> uni01; /* [0, 1]  */
};