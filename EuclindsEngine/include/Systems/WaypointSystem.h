/**
 * @file WaypointFollowSystem.h
 * @brief Sistema para gestionar el seguimiento de waypoints por parte de los corredores (racers).
 *
 * Este archivo define la estructura de configuración `WaypointFollowConfig` y la clase
 * `WaypointFollowSystem`, que implementa la lógica para que los corredores sigan una
 * ruta de puntos de ruta, con ajustes para "humanizar" el comportamiento y
 * limitar la velocidad en las curvas.
 */

#pragma once

#include <vector>
#include <limits>
#include <random>
#include <algorithm>
#include <cmath>

#include "Prerequisitos.h"1
#include "ECS/Racer.h"

// ---

/**
 * @struct WaypointFollowConfig
 * @brief Estructura de configuración para el sistema de seguimiento de waypoints.
 *
 * Contiene todos los parámetros ajustables para controlar el comportamiento de los
 * corredores (racers), incluyendo la lista de waypoints, ajustes de "humanización"
 * para evitar movimientos robóticos y parámetros para el control de velocidad en curvas.
 */
struct WaypointFollowConfig {
    /// Lista de corredores que seguirán los waypoints.
    std::vector<EngineUtilities::TSharedPointer<A_Racer>> racers;
    /// Puntero a la lista de waypoints. No es propiedad, por lo que debe ser gestionado externamente.
    const std::vector<sf::Vector2f>* waypoints = nullptr;
    /// Radio de proximidad para considerar que un corredor ha "llegado" al waypoint y avanzar al siguiente.
    float arriveRadiusForAdvance = 12.f;

    /* --- Humanización del movimiento --- */
    /// Tiempo en segundos que el corredor espera antes de reaccionar a un nuevo waypoint.
    float reactionDelay = 0.18f;
    /// Radio de aleatoriedad en píxeles que se añade al destino del waypoint para evitar la perfección robótica.
    float waypointNoiseRadius = 8.f;
    /// Número de waypoints extra que el corredor "mira" hacia adelante.
    int lookAhead = 1;
    /// Probabilidad (0.0 a 1.0) de que el corredor cometa un "error" brevemente.
    float mistakeProb = 0.05f;
    /// Tiempo de espera en segundos entre errores.
    float mistakeCooldown = 2.0f;

    /* --- Mantenimiento en la pista --- */
    /// Ancho máximo del "corredor" lateral permitido para avanzar al siguiente waypoint.
    float corridorWidth = 80.f;

    /* --- Limitador de velocidad en curvas --- */
    /// Habilita o deshabilita la reducción de velocidad en las curvas.
    bool cornerSlowdownEnabled = true;
    /// El ángulo mínimo (en grados) de una curva para que se aplique la reducción de velocidad.
    float cornerMinAngleDeg = 25.f;
    /// El ángulo máximo (en grados) de una curva para que se aplique la reducción de velocidad.
    float cornerMaxAngleDeg = 95.f;
    /// Factor de reducción de velocidad en curvas muy cerradas (ej. 0.82 = 82% de la velocidad base).
    float cornerMinFactor = 0.82f;
};

// ---

/**
 * @class WaypointFollowSystem
 * @brief Implementa la lógica para que los corredores sigan una ruta de waypoints.
 *
 * Este sistema procesa la posición de los corredores, calcula su siguiente destino
 * basado en una serie de reglas (proximidad, "humanización" y lookahead) y
 * ajusta su velocidad para manejar las curvas de forma realista.
 */
class WaypointFollowSystem {
public:
    /**
     * @brief Constructor del sistema de seguimiento de waypoints.
     * @param cfg La estructura de configuración que define los parámetros de comportamiento.
     */
    WaypointFollowSystem(const WaypointFollowConfig& cfg)
        : cfg(cfg),
        indices(cfg.racers.size(), 0),
        reactTimers(cfg.racers.size(), 0.f),
        mistakeTimers(cfg.racers.size(), 0.f),
        rng(std::random_device{}()),
        uni(-1.f, 1.f),
        uni01(0.f, 1.f) {}

    /**
     * @brief Destructor por defecto.
     */
    ~WaypointFollowSystem() = default;

    /**
     * @brief Inicializa los índices de waypoints de cada corredor.
     *
     * Asigna a cada corredor el waypoint más cercano a su posición inicial,
     * lo que evita saltos visuales al comenzar la carrera.
     */
    void primeFromPositions() {
        if (!cfg.waypoints || cfg.waypoints->empty()) {
            return;
        }

        const auto& W = *cfg.waypoints;
        for (size_t i = 0; i < cfg.racers.size(); ++i) {
            auto& r = cfg.racers[i];
            if (!r) {
                continue;
            }

            EngineMathLib::CVector2 p = r->getPosition();
            float best = std::numeric_limits<float>::max();
            size_t bestIdx = 0;

            for (size_t k = 0; k < W.size(); ++k) {
                EngineMathLib::CVector2 wk{ W[k].x, W[k].y };
                float d = (wk - p).length();
                if (d < best) {
                    best = d;
                    bestIdx = k;
                }
            }
            indices[i] = bestIdx;
            reactTimers[i] = 0.f;
            mistakeTimers[i] = 0.f;
            r->setTarget(EngineMathLib::CVector2(W[bestIdx].x, W[bestIdx].y));
        }
    }

    /**
     * @brief Actualiza el estado de todos los corredores en un fotograma.
     *
     * Este método gestiona el avance de waypoints, el comportamiento de "humanización"
     * (retrasos, errores, ruido) y el ajuste de velocidad en las curvas.
     *
     * @param dt El tiempo transcurrido desde el último fotograma.
     */
    void update(float dt) {
        if (!cfg.waypoints || cfg.waypoints->empty()) {
            return;
        }

        const auto& W = *cfg.waypoints;
        if (W.empty()) {
            return;
        }

        for (size_t i = 0; i < cfg.racers.size(); ++i) {
            auto& r = cfg.racers[i];
            if (!r) {
                continue;
            }

            /* Comportamiento "humano": retraso en la reacción */
            reactTimers[i] -= dt;
            if (reactTimers[i] > 0.f) {
                continue;
            }

            /* Índices de waypoints actuales y siguientes */
            size_t idx = indices[i];
            size_t nextIdx = (idx + 1) % W.size();
            size_t nextNextIdx = (nextIdx + 1) % W.size();

            EngineMathLib::CVector2 pos = r->getPosition();
            EngineMathLib::CVector2 wp{ W[idx].x, W[idx].y };
            EngineMathLib::CVector2 wpN{ W[nextIdx].x, W[nextIdx].y };
            EngineMathLib::CVector2 wpNN{ W[nextNextIdx].x, W[nextNextIdx].y };

            EngineMathLib::CVector2 seg = wpN - wp;
            float segLen2 = seg.x * seg.x + seg.y * seg.y;
            if (segLen2 <= 1e-4f) {
                segLen2 = 1.f;
            }

            /* Objetivo de dirección: se mira hacia adelante (+ ruido) */
            int la = std::max(0, std::min(cfg.lookAhead, 2));
            size_t tgtIdx = (idx + static_cast<size_t>(la)) % W.size();
            EngineMathLib::CVector2 target{ W[tgtIdx].x, W[tgtIdx].y };

            /* Simulación de errores ocasionales */
            mistakeTimers[i] -= dt;
            if (mistakeTimers[i] <= 0.f && uni01(rng) < cfg.mistakeProb) {
                int sign = (uni01(rng) < 0.5f) ? -1 : 1;
                size_t wrong = (tgtIdx + W.size() + static_cast<size_t>(sign)) % W.size();
                target = EngineMathLib::CVector2(W[wrong].x, W[wrong].y);
                mistakeTimers[i] = cfg.mistakeCooldown;
            }

            /* Añadir ruido al objetivo */
            if (cfg.waypointNoiseRadius > 0.f) {
                target.x += uni(rng) * cfg.waypointNoiseRadius;
                target.y += uni(rng) * cfg.waypointNoiseRadius;
            }

            /* Verificaciones de distancia y avance */
            float d_to_wp = (wp - pos).length();
            float d_to_target = (target - pos).length();

            EngineMathLib::CVector2 rel = pos - wp;
            float t = (rel.x * seg.x + rel.y * seg.y) / segLen2;
            float tClamped = std::max(0.f, std::min(1.f, t));

            EngineMathLib::CVector2 proj = EngineMathLib::CVector2(wp.x + seg.x * tClamped, wp.y + seg.y * tClamped);
            float distToLine = (proj - pos).length();

            /* Lógica de avance: */
            bool crossedForward = (t > 0.6f) && (distToLine <= cfg.corridorWidth);
            bool shouldAdvance = (d_to_wp < cfg.arriveRadiusForAdvance) || (d_to_target < (cfg.arriveRadiusForAdvance * 0.7f)) || crossedForward;

            if (shouldAdvance) {
                indices[i] = (idx + 1) % W.size();
                reactTimers[i] = cfg.reactionDelay;

                /* Actualizar el objetivo para evitar indecisiones */
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
            } else {
                /* Mantener el objetivo actual */
                if (r->getMode() != SteeringMode::Pursuit) {
                    r->setTarget(target);
                }
            }

            /* Limitador de velocidad en curvas */
            if (cfg.cornerSlowdownEnabled) {
                auto normalize = [](EngineMathLib::CVector2 v) {
                    float L = v.length();
                    if (L <= 1e-5f) {
                        return v;
                    }
                    return EngineMathLib::CVector2(v.x / L, v.y / L);
                };

                EngineMathLib::CVector2 v1 = normalize(wpN - wp);
                EngineMathLib::CVector2 v2 = normalize(wpNN - wpN);
                float dot = v1.x * v2.x + v1.y * v2.y;
                dot = std::max(-1.f, std::min(1.f, dot));
                float angleRad = std::acos(dot);
                float angleDeg = angleRad * 57.2957795f;

                float f = 1.f;
                if (angleDeg >= cfg.cornerMaxAngleDeg) {
                    f = cfg.cornerMinFactor;
                } else if (angleDeg <= cfg.cornerMinAngleDeg) {
                    f = 1.f;
                } else {
                    float tspan = (angleDeg - cfg.cornerMinAngleDeg) /
                        std::max(1.f, (cfg.cornerMaxAngleDeg - cfg.cornerMinAngleDeg));
                    f = 1.f + (cfg.cornerMinFactor - 1.f) * tspan;
                }

                float baseSpd = r->getSpeed();
                float limited = baseSpd * std::min(1.f, std::max(cfg.cornerMinFactor, f));
                if (limited < baseSpd) {
                    r->setSpeed(limited);
                }
            }
        }
    }

private:
    /// Configuración del sistema de seguimiento.
    WaypointFollowConfig cfg;
    /// Índices actuales de los waypoints para cada corredor.
    std::vector<size_t> indices;
    /// Temporizadores de reacción para cada corredor, para simular la "humanización".
    std::vector<float> reactTimers;
    /// Temporizadores de error para cada corredor.
    std::vector<float> mistakeTimers;

    /// Generador de números aleatorios.
    std::mt19937 rng;
    /// Distribución uniforme para valores en el rango [-1, 1].
    std::uniform_real_distribution<float> uni;
    /// Distribución uniforme para valores en el rango [0, 1].
    std::uniform_real_distribution<float> uni01;
};