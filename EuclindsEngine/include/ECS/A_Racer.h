#pragma once
#include "Prerequisitos.h"
#include "ECS/Actor.h"
#include "Utilities/CVector2.h"

// Modos de steering compatibles con SteeringSystem
enum class
    SteeringMode {
    Seek, Arrive, Pursuit
};

class
    A_Racer : public Actor {
public:

    A_Racer(const std::string& name);

    // Ciclo de vida
    void
        update(float deltaTime) override;

    // Meta-info opcional
    void
        setPlace(int newPlace) {
        place = newPlace;
    }
    int
        getPlace() const {
        return place;
    }

    // ---- Estado lógico de movimiento (usado por SteeringSystem / RaceSystem) ----
    void
        setPosition(const EngineMathLib::CVector2& pos) { logicPosition = pos; }
    EngineMathLib::CVector2 getPosition() const {
        return logicPosition;
    }

    void
        setTarget(const EngineMathLib::CVector2& waypoint) { logicTarget = waypoint; }
    EngineMathLib::CVector2 getTarget() const {
        return logicTarget;
    }

    void
        setSpeed(float s) { speed = s; }
    float
        getSpeed()const {
        return speed;
    }

    // ---- Steering flags & params ----
    void
        enableSteering(bool enabled) {
        steeringEnabled = enabled;
    }
    bool
        isSteeringEnabled() const {
        return steeringEnabled;
    }

    void
        setArriveRadius(float radius) { arriveRadius = radius; }
    float
        getArriveRadius() const {
        return arriveRadius;
    }

    void
        setMode(SteeringMode m) { mode = m; }
    SteeringMode getMode() const {
        return mode;
    }

    // Pursuit
    void
        setPursuitTarget(const EngineUtilities::TWeakPointer<A_Racer>& t) { pursuitTarget = t; }
    EngineUtilities::TWeakPointer<A_Racer> getPursuitTarget() const { return pursuitTarget; }

    void
        setMaxPrediction(float p) {
        maxPrediction = p;
    }
    float
        getMaxPrediction()  const {
        return maxPrediction;
    }

private:
    // ranking opcional
    int   place = 0;

    // estado lógico
    EngineMathLib::CVector2 logicPosition{ 0.f, 0.f };
    EngineMathLib::CVector2 logicTarget{ 0.f, 0.f };
    float speed = 100.f;

    // steering
    bool  steeringEnabled = true;
    float arriveRadius = 10.f;
    SteeringMode mode = SteeringMode::Seek;

    // pursuit
    EngineUtilities::TWeakPointer<A_Racer> pursuitTarget;
    float maxPrediction = 0.5f; // segundos
};