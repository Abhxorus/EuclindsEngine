#pragma once

#include "Prerequisitos.h"
#include "ECS/Actor.h"
#include "Utilities/CVector2.h"

/**
 * @file A_Racer.h
 * @brief Declaración del actor de tipo corredor y modos de steering compatibles con SteeringSystem.
 *
 * Contiene el estado lógico de movimiento, parámetros de steering y datos opcionales
 * como la posición en la carrera.
 *
 * @see Actor
 * @see SteeringMode
 */

 /**
  * @enum SteeringMode
  * @brief Modos de steering soportados por el sistema.
  *
  * - SteeringMode::Seek: Avanza directamente hacia un objetivo.
  * - SteeringMode::Arrive: Se aproxima a un objetivo reduciendo la velocidad.
  * - SteeringMode::Pursuit: Persigue a otro actor en movimiento.
  */
enum class SteeringMode {
    /// Movimiento directo hacia un objetivo.
    Seek,
    /// Movimiento con desaceleración al llegar.
    Arrive,
    /// Persecución de otro actor.
    Pursuit
};

/**
 * @class A_Racer
 * @brief Actor que representa un corredor controlado por sistemas de steering.
 *
 * Maneja posición lógica, velocidad, objetivo, y parámetros de steering como radio de llegada,
 * modo actual y datos de persecución.
 */
class A_Racer : public Actor {
public:
    /**
     * @brief Constructor del corredor.
     * @param name Nombre lógico del actor.
     */
    A_Racer(const std::string& name);

    // ----- Ciclo de vida -----

    /**
     * @brief Actualiza el estado del corredor.
     * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
     */
    void update(float deltaTime) override;

    // ----- Meta-info opcional -----

    /**
     * @brief Establece la posición en la carrera.
     * @param newPlace Nueva posición (ranking).
     */
    void setPlace(int newPlace) { place = newPlace; }

    /**
     * @brief Obtiene la posición en la carrera.
     * @return Posición (ranking).
     */
    int getPlace() const { return place; }

    // ----- Estado lógico de movimiento -----

    /**
     * @brief Fija la posición lógica del corredor.
     * @param pos Coordenadas en el plano.
     */
    void setPosition(const EngineMathLib::CVector2& pos) { logicPosition = pos; }

    /**
     * @brief Obtiene la posición lógica del corredor.
     * @return Coordenadas actuales.
     */
    EngineMathLib::CVector2 getPosition() const { return logicPosition; }

    /**
     * @brief Fija el objetivo lógico del corredor.
     * @param waypoint Coordenadas del objetivo.
     */
    void setTarget(const EngineMathLib::CVector2& waypoint) { logicTarget = waypoint; }

    /**
     * @brief Obtiene el objetivo lógico actual.
     * @return Coordenadas del objetivo.
     */
    EngineMathLib::CVector2 getTarget() const { return logicTarget; }

    /**
     * @brief Establece la velocidad actual.
     * @param s Velocidad en unidades/segundo.
     */
    void setSpeed(float s) { speed = s; }

    /**
     * @brief Obtiene la velocidad actual.
     * @return Velocidad en unidades/segundo.
     */
    float getSpeed() const { return speed; }

    // ----- Steering -----

    /**
     * @brief Activa o desactiva el steering.
     * @param enabled true para habilitar, false para deshabilitar.
     */
    void enableSteering(bool enabled) { steeringEnabled = enabled; }

    /**
     * @brief Comprueba si el steering está habilitado.
     * @return true si está habilitado, false en caso contrario.
     */
    bool isSteeringEnabled() const { return steeringEnabled; }

    /**
     * @brief Establece el radio de llegada (modo Arrive).
     * @param radius Radio en unidades.
     */
    void setArriveRadius(float radius) { arriveRadius = radius; }

    /**
     * @brief Obtiene el radio de llegada (modo Arrive).
     * @return Radio en unidades.
     */
    float getArriveRadius() const { return arriveRadius; }

    /**
     * @brief Define el modo de steering.
     * @param m Modo de steering (@ref SteeringMode).
     */
    void setMode(SteeringMode m) { mode = m; }

    /**
     * @brief Obtiene el modo de steering actual.
     * @return Modo de steering.
     */
    SteeringMode getMode() const { return mode; }

    // ----- Pursuit -----

    /**
     * @brief Define el objetivo de persecución.
     * @param t Puntero débil al actor objetivo.
     */
    void setPursuitTarget(const EngineUtilities::TWeakPointer<A_Racer>& t) { pursuitTarget = t; }

    /**
     * @brief Obtiene el objetivo de persecución.
     * @return Puntero débil al actor objetivo.
     */
    EngineUtilities::TWeakPointer<A_Racer> getPursuitTarget() const { return pursuitTarget; }

    /**
     * @brief Establece el tiempo máximo de predicción (pursuit).
     * @param p Tiempo en segundos.
     */
    void setMaxPrediction(float p) { maxPrediction = p; }

    /**
     * @brief Obtiene el tiempo máximo de predicción (pursuit).
     * @return Tiempo en segundos.
     */
    float getMaxPrediction() const { return maxPrediction; }

private:
    /// Posición en la carrera (ranking).
    int place = 0;

    /// Posición lógica en el plano.
    EngineMathLib::CVector2 logicPosition{ 0.f, 0.f };

    /// Objetivo lógico.
    EngineMathLib::CVector2 logicTarget{ 0.f, 0.f };

    /// Velocidad actual.
    float speed = 100.f;

    /// Estado del steering.
    bool steeringEnabled = true;

    /// Radio de llegada (modo Arrive).
    float arriveRadius = 10.f;

    /// Modo de steering actual.
    SteeringMode mode = SteeringMode::Seek;

    /// Objetivo de persecución.
    EngineUtilities::TWeakPointer<A_Racer> pursuitTarget;

    /// Tiempo máximo de predicción para pursuit (segundos).
    float maxPrediction = 0.5f;
};
