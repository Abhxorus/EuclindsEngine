#pragma once
#include "Prerequisitos.h"
#include "ECS/A_Racer.h"
#include "ECS/Transform.h"

// Config para mover racers (NPCs y, opcional, player si está en TargetSeek)
struct
    SteeringConfig {
    std::vector<EngineUtilities::TSharedPointer<A_Racer>> racers;
    float seekRange = 10.f; // umbral de parada para Transform::seek
};

class
    SteeringSystem {
public:
    SteeringSystem(const SteeringConfig& cfg) : cfg(cfg) {}

    ~SteeringSystem() = default;

    void
        update(float dt);

private:
    SteeringConfig cfg;

    // Lógica para Arrive y Pursuit (CVector2)
    EngineMathLib::CVector2 arrive_L(const EngineMathLib::CVector2& pos,
        const EngineMathLib::CVector2& target,
        float speed, float arriveRadius);

    EngineMathLib::CVector2 pursuit_L(const A_Racer& self,
        const A_Racer& target,
        float speed, float maxPrediction);

    static
        sf::Vector2f toSF(const EngineMathLib::CVector2& v) {
        return { v.x, v.y };
    }
    static
        EngineMathLib::CVector2 toC(const sf::Vector2f& v) {
        return { v.x, v.y };
    }
};