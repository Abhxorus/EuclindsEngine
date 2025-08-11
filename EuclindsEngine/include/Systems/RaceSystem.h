#pragma once
#include "Prerequisitos.h"
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "Components/LapComponent.h"
#include "ECS/A_Racer.h"
#include <vector>

/**
 * @file RaceSystem.h
 * @brief Sistema encargado de gestionar una carrera, incluyendo vueltas, posiciones y cronometraje.
 *
 * Controla el progreso de todos los actores (jugador y NPCs) en un circuito cerrado,
 * contabiliza vueltas, tiempos y determina la clasificación.
 *
 * @see Actor
 * @see LapComponent
 * @see A_Racer
 */

 /**
  * @struct RaceConfig
  * @brief Configuración de la carrera.
  *
  * Contiene referencias a los actores participantes, puntos de control (waypoints)
  * y parámetros de detección de checkpoints.
  */
struct RaceConfig {
    /// Actores participantes (jugador + NPCs).
    std::vector<EngineUtilities::TSharedPointer<Actor>> actors;

    /// Puntero a los waypoints que definen el circuito cerrado.
    const std::vector<sf::Vector2f>* waypoints = nullptr;

    /// Radio de detección de checkpoint (en unidades).
    float checkpointRadius = 12.f;
};

/**
 * @class RaceSystem
 * @brief Gestiona la lógica de una carrera.
 *
 * Lleva el control del progreso de cada actor en el circuito, calcula vueltas,
 * tiempos, posiciones y determina la clasificación.
 */
class RaceSystem {
public:
    /**
     * @brief Constructor.
     * @param cfg Configuración inicial de la carrera.
     */
    RaceSystem(const RaceConfig& cfg);

    /**
     * @brief Destructor por defecto.
     */
    ~RaceSystem() = default;

    /**
     * @brief Actualización por frame.
     * @param dt Tiempo transcurrido desde el último frame (segundos).
     */
    void update(float dt);

    /**
     * @brief Obtiene la clasificación actual.
     * @return Vector con los índices en cfg.actors, ordenados por progreso descendente.
     */
    std::vector<int> getStandings() const;

    /**
     * @brief Acceso al componente LapComponent de un actor.
     * @param i Índice del actor en cfg.actors.
     * @return Referencia constante a su LapComponent.
     */
    const LapComponent& getLapData(size_t i) const {
        return laps[i];
    }

    /**
     * @brief Activa o desactiva el cronometraje.
     * @param b true para activar, false para desactivar.
     * @note Útil para ignorar tiempo durante una cuenta regresiva.
     */
    void setTimingActive(bool b) {
        timingActive = b;
    }

    /**
     * @brief Obtiene el tiempo actual de la vuelta del jugador.
     * @return Tiempo en segundos.
     */
    float getPlayerCurrentLapTime() const {
        return playerLapTime;
    }

    /**
     * @brief Obtiene el mejor tiempo de vuelta del jugador.
     * @return Tiempo en segundos, o -1.f si no hay vuelta válida.
     */
    float getPlayerBestLapTime() const {
        return (bestLapValid ? playerBestLap : -1.f);
    }

    /**
     * @brief Indica si hay un mejor tiempo de vuelta registrado.
     * @return true si hay un mejor tiempo válido.
     */
    bool hasBestLap() const {
        return bestLapValid;
    }

private:
    /// Configuración de la carrera.
    RaceConfig cfg;

    /// Datos de vueltas para cada actor.
    std::vector<LapComponent> laps;

    /// Progreso absoluto (metros) = vuelta * longitud total + s.
    std::vector<float> progress;

    /// Tiempo transcurrido por actor (segundos).
    std::vector<float> elapsed;

    /// Posición previa a lo largo del circuito (para detectar vueltas).
    std::vector<float> lastS;

    /// Índice del actor al que se le cuentan vueltas (jugador).
    int lapOwnerIndex = 0;

    /// Tiempo mínimo entre detecciones de meta para evitar rebotes.
    float lapDebounceSec = 0.35f;

    /// Temporizador antirrebote (solo jugador).
    float lapCooldown = 0.f;

    /// Primer cruce arma, no suma.
    bool lapArmed = false;

    /// Estado del cronometraje.
    bool timingActive = false;

    /// Tiempo actual de vuelta del jugador.
    float playerLapTime = 0.f;

    /// Mejor tiempo de vuelta del jugador.
    float playerBestLap = 0.f;

    /// Indica si hay mejor vuelta válida.
    bool bestLapValid = false;

    /// Longitudes acumuladas de cada segmento del circuito.
    std::vector<float> prefix;

    /// Longitud total del circuito.
    float totalLen = 0.f;

    /// Indica si el circuito es cerrado.
    bool closedLoop = true;

    /**
     * @brief Obtiene la posición de un actor en coordenadas CVector2.
     * @param a Puntero compartido al actor.
     */
    static EngineMathLib::CVector2 getActorPos(const EngineUtilities::TSharedPointer<Actor>& a);

    /**
     * @brief Convierte un vector SFML a CVector2.
     * @param v Vector 2D de SFML.
     * @return Vector 2D de EngineMathLib.
     */
    static EngineMathLib::CVector2 toC(const sf::Vector2f& v) {
        return { v.x, v.y };
    }

    /**
     * @brief Calcula el progreso relativo de un punto dentro de un segmento.
     * @param p Punto a evaluar.
     * @param a Inicio del segmento.
     * @param b Fin del segmento.
     * @return Progreso en metros relativo al segmento.
     */
    float segProgress(const EngineMathLib::CVector2& p,
        const EngineMathLib::CVector2& a,
        const EngineMathLib::CVector2& b) const;

    /**
     * @brief Calcula la distancia a lo largo del circuito desde el inicio.
     * @param p Punto a evaluar.
     * @return Distancia recorrida en metros.
     */
    float sAlongPath(const EngineMathLib::CVector2& p) const;

    /**
     * @brief Calcula las distancias acumuladas para cada punto del circuito.
     */
    void buildCircuitMeter();

    /**
     * @brief Calcula la distancia entre dos puntos (sf::Vector2f).
     */
    static float dist(const sf::Vector2f& A, const sf::Vector2f& B) {
        float dx = B.x - A.x, dy = B.y - A.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    /**
     * @brief Calcula la distancia entre dos puntos (CVector2).
     */
    static float distC(const EngineMathLib::CVector2& A, const EngineMathLib::CVector2& B) {
        float dx = B.x - A.x, dy = B.y - A.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};
