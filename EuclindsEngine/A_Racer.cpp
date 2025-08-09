#include "ECS/A_Racer.h"

A_Racer::A_Racer(const std::string& name)
    : Actor(name), place(0), speed(100.f),
    logicPosition(EngineMathLib::CVector2::zero()),
    logicTarget(EngineMathLib::CVector2::zero()),
    steeringEnabled(true),
    arriveRadius(10.f)
{
}

void A_Racer::update(float deltaTime) {
    if (steeringEnabled) {
        EngineMathLib::CVector2 toTarget = logicTarget - logicPosition;
        float distance = toTarget.length();

        if (distance > arriveRadius) {
            EngineMathLib::CVector2 direction = toTarget.normalized();
            logicPosition += direction * speed * deltaTime;
        }
    }
    // Sincronizar con Transform para el render
    auto transform = getComponent<Transform>();
    if (transform) {
        transform->setPosition(EngineMathLib::CVector2::cVector2ToSF(logicPosition));
    }
    Actor::update(deltaTime);
}

void A_Racer::setPlace(int newPlace) { place = newPlace; }
int A_Racer::getPlace() const { return place; }

void A_Racer::setPosition(const EngineMathLib::CVector2& pos) { logicPosition = pos; }
EngineMathLib::CVector2 A_Racer::getPosition() const { return logicPosition; }

void A_Racer::setSpeed(float s) { speed = s; }
float A_Racer::getSpeed() const { return speed; }

void A_Racer::setTarget(const EngineMathLib::CVector2& waypoint) { logicTarget = waypoint; }
EngineMathLib::CVector2 A_Racer::getTarget() const { return logicTarget; }

void A_Racer::enableSteering(bool enabled) { steeringEnabled = enabled; }
void A_Racer::setArriveRadius(float radius) { arriveRadius = radius; }