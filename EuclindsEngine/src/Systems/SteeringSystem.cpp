#include "Systems/SteeringSystem.h"

void
SteeringSystem::update(float dt) {
    for (auto& r : cfg.racers) {
        if (!r || !r->isSteeringEnabled()) continue;

        const auto speed = r->getSpeed();
        const auto target = r->getTarget();
        auto tr = r->getComponent<Transform>();

        switch (r->getMode()) {
        case SteeringMode::Seek: {
            // Reusar Transform::seek y sincronizar lógica
            if (tr) {
                tr->seek(toSF(target), speed, dt, cfg.seekRange);
                r->setPosition(toC(tr->getPosition()));
            }
            break;
        }
        case SteeringMode::Arrive: {
            auto posL = r->getPosition();
            auto vel = arrive_L(posL, target, speed, r->getArriveRadius());
            posL = posL + vel * dt;
            r->setPosition(posL);
            if (tr) tr->setPosition(toSF(posL));
            break;
        }
        case SteeringMode::Pursuit: {
            auto posL = r->getPosition();
            if (auto t = r->getPursuitTarget().lock()) {
                auto vel = pursuit_L(*r, *t, speed, r->getMaxPrediction());
                posL = posL + vel * dt;
                r->setPosition(posL);
                if (tr) tr->setPosition(toSF(posL));
            }
            else if (tr) {
                tr->seek(toSF(target), speed, dt, cfg.seekRange);
                r->setPosition(toC(tr->getPosition()));
            }
            break;
        }
        }
    }
}

EngineMathLib::CVector2 SteeringSystem::arrive_L(
    const EngineMathLib::CVector2& pos,
    const EngineMathLib::CVector2& target, float speed, float arriveRadius) {

    auto toT = target - pos;
    float d = toT.length();
    if (d < 1e-4f) return { 0.f, 0.f };
    float s = (d < arriveRadius) ? speed * (d / arriveRadius) : speed;
    return (toT / d) * s;
}

EngineMathLib::CVector2 SteeringSystem::pursuit_L(
    const A_Racer& self, const A_Racer& target, float speed, float maxPrediction) {

    auto toT = target.getPosition() - self.getPosition();
    float d = toT.length();
    float pred = std::min(maxPrediction, d / std::max(1.f, target.getSpeed()));
    auto fut = target.getPosition() + toT.normalized() * target.getSpeed() * pred;

    auto aim = fut - self.getPosition();
    float L = aim.length();
    if (L < 1e-4f) return { 0.f, 0.f };
    return (aim / L) * speed;
}