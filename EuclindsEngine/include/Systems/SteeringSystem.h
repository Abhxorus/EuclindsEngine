#pragma once
#include "Prerequisitos.h"
#include "ECS/Racer.h"
#include "ECS/Transform.h"

/**
 * @file SteeringSystem.h
 * @brief Sistema encargado de aplicar comportamientos de steering (Seek, Arrive, Pursuit) a corredores.
 *
 * Este sistema procesa el movimiento de NPCs y, opcionalmente, del jugador si está en modo TargetSeek.
 */

 /**
  * @struct SteeringConfig
  * @brief Configuración inicial para el sistema de steering.
  *
  * Contiene la lista de corredores a mover y parámetros como el rango de detención para Seek.
  */
struct SteeringConfig {
    /// Lista de corredores que serán procesados por el sistema.
    std::vector<EngineUtilities::TSharedPointer<A_Racer>> racers;

    /// Umbral de distancia para detenerse en modo Seek.
    float seekRange = 10.f; ///< Unidades de Transform::seek.
};

/**
 * @class SteeringSystem
 * @brief Aplica comportamientos de steering a una lista de corredores.
 *
 * Gestiona movimientos en modos Arrive y Pursuit, realizando cálculos vectoriales para determinar
 * la dirección y velocidad objetivo de cada corredor.
 */
class SteeringSystem {
public:
    /**
     * @brief Constructor.
     * @param cfg Configuración inicial del sistema.
     */
    SteeringSystem(const SteeringConfig& cfg) : cfg(cfg) {}

    /**
     * @brief Destructor por defecto.
     */
    ~SteeringSystem() = default;

    /**
     * @brief Actualiza el sistema por frame.
     * @param dt Delta de tiempo desde el último frame (segundos).
     */
    void update(float dt);

private:
    /// Configuración del sistema.
    SteeringConfig cfg;

    /**
     * @brief Calcula el vector de movimiento usando el comportamiento Arrive.
     * @param pos Posición actual.
     * @param target Posición objetivo.
     * @param speed Velocidad máxima.
     * @param arriveRadius Radio en el que comienza a desacelerar.
     * @return Vector de dirección y magnitud para moverse.
     */
    EngineMathLib::CVector2 arrive_L(const EngineMathLib::CVector2& pos,
        const EngineMathLib::CVector2& target,
        float speed, float arriveRadius);

    /**
     * @brief Calcula el vector de movimiento usando el comportamiento Pursuit.
     * @param self Corredor que persigue.
     * @param target Corredor objetivo.
     * @param speed Velocidad máxima.
     * @param maxPrediction Tiempo máximo de predicción (segundos).
     * @return Vector de dirección y magnitud para moverse.
     */
    EngineMathLib::CVector2 pursuit_L(const A_Racer& self,
        const A_Racer& target,
        float speed, float maxPrediction);

    /**
     * @brief Convierte un vector de EngineMathLib a sf::Vector2f.
     * @param v Vector en formato CVector2.
     * @return Vector en formato sf::Vector2f.
     */
    static sf::Vector2f toSF(const EngineMathLib::CVector2& v) {
        return { v.x, v.y };
    }

    /**
     * @brief Convierte un vector de sf::Vector2f a EngineMathLib::CVector2.
     * @param v Vector en formato sf::Vector2f.
     * @return Vector en formato CVector2.
     */
    static EngineMathLib::CVector2 toC(const sf::Vector2f& v) {
        return { v.x, v.y };
    }
};
