#pragma once
#include "Prerequisitos.h"
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "Utilities/CVector2.h"

/**
 * @file A_Player.h
 * @brief Declaración del actor controlable por el jugador y su modo de control.
 *
 * Esta clase expone parámetros de control directo (WASD) y por objetivo (target seek),
 * utilizando el componente Transform como fuente de verdad para la posición.
 *
 * @see Actor
 * @see Transform
 */

 /// Modo de control del jugador.
 /**
  * @enum PlayerControlMode
  * @brief Determina cómo se calcula el movimiento del jugador.
  *
  * - PlayerControlMode::Direct: control directo tipo WASD, afectado por aceleración, fricción y velocidad máxima.
  * - PlayerControlMode::TargetSeek: el jugador se mueve hacia un objetivo (target).
  */
enum class PlayerControlMode {
    /// Control directo (WASD).
    Direct,
    /// Búsqueda de objetivo (seek a target).
    TargetSeek
};

/**
 * @class A_Player
 * @brief Actor controlable por el usuario.
 *
 * Expone getters/setters para parámetros de movimiento y acceso a posición/velocidad, manteniendo
 * el componente Transform como fuente de verdad de la posición en el mundo.
 */
class A_Player : public Actor {
public:
    /**
     * @brief Crea un jugador con un nombre dado.
     * @param name Nombre lógico del actor.
     */
    A_Player(const std::string& name) : Actor(name) {}

    // ----- Control mode -----

    /**
     * @brief Establece el modo de control del jugador.
     * @param m Modo de control (@ref PlayerControlMode).
     */
    void setControlMode(PlayerControlMode m) {
        controlMode = m;
    }

    /**
     * @brief Obtiene el modo de control actual.
     * @return Modo de control (@ref PlayerControlMode).
     */
    PlayerControlMode getControlMode() const {
        return controlMode;
    }

    // ----- Parámetros para control directo (WASD) -----

    /**
     * @brief Establece la aceleración.
     * @param a Aceleración en px/s^2.
     * @note Afecta la rapidez con la que se incrementa la velocidad.
     */
    void setAcceleration(float a) {
        acceleration = a;
    }

    /**
     * @brief Obtiene la aceleración.
     * @return Aceleración en px/s^2.
     */
    float getAcceleration() const {
        return acceleration;
    }

    /**
     * @brief Establece la fricción (factor de desaceleración).
     * @param f Coeficiente de fricción (adimensional).
     * @note Valores mayores implican detenciones más rápidas.
     */
    void setFriction(float f) {
        friction = f;
    }

    /**
     * @brief Obtiene la fricción (factor de desaceleración).
     * @return Coeficiente de fricción (adimensional).
     */
    float getFriction() const {
        return friction;
    }

    /**
     * @brief Establece la velocidad máxima permitida.
     * @param s Velocidad máxima en px/s.
     */
    void setMaxSpeed(float s) {
        maxSpeed = s;
    }

    /**
     * @brief Obtiene la velocidad máxima permitida.
     * @return Velocidad máxima en px/s.
     */
    float getMaxSpeed() const {
        return maxSpeed;
    }

    /**
     * @brief Fija la velocidad lineal actual.
     * @param v Velocidad 2D (px/s).
     */
    void setVelocity(const EngineMathLib::CVector2& v) { velocity = v; }

    /**
     * @brief Devuelve la velocidad lineal actual.
     * @return Velocidad 2D (px/s).
     */
    EngineMathLib::CVector2 getVelocity() const { return velocity; }

    // ----- Posición/target usando Transform como fuente de verdad -----

    /**
     * @brief Obtiene la posición actual desde el componente Transform.
     * @return Posición 2D en px. Si no existe Transform, retorna {0.f, 0.f}.
     * @warning Si el Actor no tiene componente Transform adjunto, se devuelve (0,0).
     */
    EngineMathLib::CVector2 getPosition() {
        auto tr = getComponent<Transform>();
        if (tr) return { tr->getPosition().x, tr->getPosition().y };
        return { 0.f, 0.f };
    }

    /**
     * @brief Establece la posición en el componente Transform (si existe).
     * @param p Posición 2D en px.
     * @note No hace nada si el Actor no tiene un Transform adjunto.
     */
    void setPosition(const EngineMathLib::CVector2& p) {
        if (auto tr = getComponent<Transform>()) tr->setPosition({ p.x, p.y });
    }

    /**
     * @brief Define el objetivo hacia el cual se desplazará el jugador en modo TargetSeek.
     * @param t Coordenadas del objetivo (px).
     */
    void setTarget(const EngineMathLib::CVector2& t) { target = t; }

    /**
     * @brief Obtiene el objetivo actual del jugador.
     * @return Coordenadas del objetivo (px).
     */
    EngineMathLib::CVector2 getTarget() const { return target; }

private:
    /// Modo de control actual.
    PlayerControlMode       controlMode = PlayerControlMode::Direct;

    /// Velocidad lineal actual (px/s).
    EngineMathLib::CVector2 velocity{ 0.f, 0.f };

    /// Aceleración (px/s^2).
    float                   acceleration = 600.f;

    /// Coeficiente de fricción (adimensional).
    float                   friction = 6.f;

    /// Velocidad máxima (px/s).
    float                   maxSpeed = 250.f;

    /// Objetivo hacia el cual se mueve el jugador en modo TargetSeek (px).
    EngineMathLib::CVector2 target{ 0.f, 0.f };
};
