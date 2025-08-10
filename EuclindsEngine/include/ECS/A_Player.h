#pragma once
#include "Prerequisitos.h"
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "Utilities/CVector2.h"

// Modo de control del jugador
enum class
    PlayerControlMode {
    Direct, TargetSeek
};

class
    A_Player : public Actor {
public:

    A_Player(const std::string& name) : Actor(name) {}

    // Control mode
    void
        setControlMode(PlayerControlMode m) {
        controlMode = m;
    }

    PlayerControlMode
        getControlMode() const {
        return controlMode;
    }

    // Parámetros para control directo (WASD)
    void
        setAcceleration(float a) {
        acceleration = a;
    }
    float
        getAcceleration() const {
        return acceleration;
    }

    void
        setFriction(float f) {
        friction = f;
    }

    float
        getFriction() const {
        return friction;
    }

    void
        setMaxSpeed(float s) {
        maxSpeed = s;
    }

    float
        getMaxSpeed() const {
        return maxSpeed;
    }

    void
        setVelocity(const EngineMathLib::CVector2& v) { velocity = v; }
    EngineMathLib::CVector2 getVelocity() const { return velocity; }

    // Posición/target usando Transform como fuente de verdad del player
    EngineMathLib::CVector2 getPosition() {
        auto tr = getComponent<Transform>();
        if (tr) return { tr->getPosition().x, tr->getPosition().y };
        return { 0.f, 0.f };
    }

    void
        setPosition(const EngineMathLib::CVector2& p) {
        if (auto tr = getComponent<Transform>()) tr->setPosition({ p.x, p.y });
    }

    void
        setTarget(const EngineMathLib::CVector2& t) { target = t; }
    EngineMathLib::CVector2 getTarget() const { return target; }

private:
    PlayerControlMode       controlMode = PlayerControlMode::Direct;
    EngineMathLib::CVector2 velocity{ 0.f, 0.f };
    float                   acceleration = 600.f; // px/s^2
    float                   friction = 6.f;
    float                   maxSpeed = 250.f;
    EngineMathLib::CVector2 target{ 0.f, 0.f };
};