/**
 * @file PlayerInputSystem.cpp
 * @brief Implementación del sistema de entrada del jugador para controlar un actor.
 *
 * Este archivo contiene la lógica para procesar la entrada del teclado y del ratón
 * y aplicarla a un actor, permitiendo dos modos de control: directo (con las teclas)
 * y de búsqueda de objetivo (con el ratón).
 */

#include "Systems/PlayerInputSystem.h"
#include "ECS/Transform.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

 /**
  * @brief Actualiza la lógica de entrada del jugador.
  *
  * Se encarga de llamar a la función de actualización apropiada según
  * el modo de control actual del jugador.
  *
  * @param dt El tiempo transcurrido desde el último fotograma.
  */
void PlayerInputSystem::update(float dt) {
    if (!cfg.player) {
        return;
    }

    if (cfg.player->getControlMode() == PlayerControlMode::Direct) {
        updateDirect(dt);
    }
    else {
        updateTargetSeek();
    }
}

/**
 * @brief Actualiza el control de movimiento directo del jugador.
 *
 * Responde a las pulsaciones de las teclas W, A, S, D o las flechas para
 * aplicar aceleración y fricción al actor, controlando su velocidad y posición.
 *
 * @param dt El tiempo transcurrido desde el último fotograma.
 */
void PlayerInputSystem::updateDirect(float dt) {
    EngineMathLib::CVector2 a(0.f, 0.f);

    // Detección de teclas de movimiento
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        a.y -= cfg.player->getAcceleration();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        a.y += cfg.player->getAcceleration();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        a.x -= cfg.player->getAcceleration();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        a.x += cfg.player->getAcceleration();
    }

    auto v = cfg.player->getVelocity();
    v += a * dt;

    // Aplicar fricción si no hay entrada de movimiento
    if (a.x == 0.f && a.y == 0.f) {
        v *= std::max(0.f, 1.f - cfg.player->getFriction() * dt);
    }

    // Limitar la velocidad máxima
    float sp = v.length();
    if (sp > cfg.player->getMaxSpeed()) {
        v = v.normalized() * cfg.player->getMaxSpeed();
    }

    // Integrar y actualizar la posición
    auto p = cfg.player->getPosition();
    p += v * dt;

    cfg.player->setVelocity(v);
    cfg.player->setPosition(p);
}

/**
 * @brief Actualiza el modo de búsqueda de objetivo del jugador.
 *
 * Si el botón izquierdo del ratón está presionado, establece la posición del cursor
 * como el nuevo objetivo del actor.
 */
void PlayerInputSystem::updateTargetSeek() {
    if (!cfg.renderWindow) {
        return;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        auto mp = sf::Mouse::getPosition(*cfg.renderWindow);
        auto world = cfg.renderWindow->mapPixelToCoords(mp);
        cfg.player->setTarget({ world.x, world.y });
    }
}