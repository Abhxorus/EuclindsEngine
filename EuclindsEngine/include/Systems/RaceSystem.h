#pragma once
#include "Prerequisitos.h"
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "Components/LapComponent.h"
#include "ECS/A_Racer.h"
#include <vector>

/*
 * RaceConfig
 * - actors: Player + NPCs
 * - waypoints: closed circuit
 */
    struct
    RaceConfig {
    std::vector<EngineUtilities::TSharedPointer<Actor>> actors;  /* Player + NPCs */
    const std::vector<sf::Vector2f>* waypoints = nullptr;        /* closed circuit */
    float checkpointRadius = 12.f;
};

class
    RaceSystem {
public:

    RaceSystem(const RaceConfig& cfg);
    ~RaceSystem() = default;

    /* per-frame update */
    void
        update(float dt);

    /* standings: indices en cfg.actors, ordenados por progress descendente */
    std::vector<int> getStandings() const;

    /* acceso al lap data crudo */
    const LapComponent&
        getLapData(size_t i) const {
        return laps[i];
    }

    /* control del cronometraje (para no contar durante countdown) */
    void
        setTimingActive(bool b) {
        timingActive = b;
    }

    /* lecturas de tiempo para HUD */
    float
        getPlayerCurrentLapTime() const {
        return playerLapTime;
    }              /* segundos */
    float
        getPlayerBestLapTime()    const {
        return (bestLapValid ? playerBestLap : -1.f);
    }

    bool hasBestLap()  const {
        return bestLapValid;
    }

private:
    RaceConfig cfg;
    std::vector<LapComponent> laps;     /* one per actor */
    std::vector<float> progress;        /* absolute progress (meters) = lap * totalLen + s */
    std::vector<float> elapsed;         /* elapsed time per actor (seconds) */
    std::vector<float> lastS;           /* previous sAlongPath (wrap detect) */

    /* SOLO contamos vueltas para el player (index 0) */
    int   lapOwnerIndex = 0;
    float lapDebounceSec = 0.35f;       /* antirrebote meta */
    float lapCooldown = 0.f;         /* temporizador antirrebote (solo player) */
    bool  lapArmed = false;       /* primer cruce arma, no suma */

    /* timing */
    bool  timingActive = false;       /* activado desde BaseApp al dar GO */
    float playerLapTime = 0.f;         /* corriendo */
    float playerBestLap = 0.f;         /* mejor vuelta */
    bool  bestLapValid = false;

    /* circuit meter */
    std::vector<float> prefix;
    float totalLen = 0.f;
    bool  closedLoop = true;

    static EngineMathLib::CVector2
        getActorPos(const EngineUtilities::TSharedPointer<Actor>& a);

    static EngineMathLib::CVector2
        toC(const sf::Vector2f& v) {
        return { v.x, v.y };
    }

    float
        segProgress(const EngineMathLib::CVector2& p,
            const EngineMathLib::CVector2& a,
            const EngineMathLib::CVector2& b) const;

    float
        sAlongPath(const EngineMathLib::CVector2& p) const;

    void
        buildCircuitMeter();

    static float
        dist(const sf::Vector2f& A, const sf::Vector2f& B) {
        float dx = B.x - A.x, dy = B.y - A.y; return std::sqrt(dx * dx + dy * dy);
    }
    static float
        distC(const EngineMathLib::CVector2& A, const EngineMathLib::CVector2& B) {
        float dx = B.x - A.x, dy = B.y - A.y; return std::sqrt(dx * dx + dy * dy);
    }
};