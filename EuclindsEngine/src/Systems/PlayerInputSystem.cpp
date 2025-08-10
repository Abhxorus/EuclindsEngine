#include "Systems/PlayerInputSystem.h"
#include "ECS/Transform.h"
#include <SFML/Window/Keyboard.hpp> // Ensure this header is included for sf::Keyboard

void
PlayerInputSystem::update(float dt) {
    if (!cfg.player) return;

    if (cfg.player->getControlMode() == PlayerControlMode::Direct) {
        updateDirect(dt);
    }
    else {
        updateTargetSeek();
    }
}

void
PlayerInputSystem::updateDirect(float dt) {
    EngineMathLib::CVector2 a(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        a.y -= cfg.player->getAcceleration();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        a.y += cfg.player->getAcceleration();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        a.x -= cfg.player->getAcceleration();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        a.x += cfg.player->getAcceleration();

    auto v = cfg.player->getVelocity();
    v += a * dt;

    // Fricción cuando no hay input
    if (a.x == 0.f && a.y == 0.f) {
        v *= std::max(0.f, 1.f - cfg.player->getFriction() * dt);
    }

    // Clamp
    float sp = v.length();
    if (sp > cfg.player->getMaxSpeed()) v = v.normalized() * cfg.player->getMaxSpeed();

    // Integrar
    auto p = cfg.player->getPosition();
    p += v * dt;

    cfg.player->setVelocity(v);
    cfg.player->setPosition(p);
}

void
PlayerInputSystem::updateTargetSeek() {
    if (!cfg.renderWindow) return;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        auto mp = sf::Mouse::getPosition(*cfg.renderWindow);
        auto world = cfg.renderWindow->mapPixelToCoords(mp);
        cfg.player->setTarget({ world.x, world.y });
    }
}