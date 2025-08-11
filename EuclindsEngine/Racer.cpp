/**
 * @file A_Racer.cpp
 * @brief Implementación de la clase A_Racer.
 *
 * A_Racer es una subclase de Actor que representa a un corredor en el juego.
 * Contiene la lógica específica para el movimiento y comportamiento de los corredores.
 */
#include "ECS/Racer.h"
#include "ECS/Transform.h"

 /**
  * @brief Constructor de la clase A_Racer.
  * @param name El nombre que se le asignará al actor.
  */
A_Racer::A_Racer(const std::string& name)
    : Actor(name) {
    // El constructor delega en la clase base Actor
}

/**
 * @brief Actualiza la lógica del corredor en cada fotograma.
 *
 * Sincroniza la posición de la lógica del juego (`logicPosition`) con el
 * componente de transformación (`Transform`) del actor.
 *
 * @param deltaTime El tiempo transcurrido desde el último fotograma.
 */
void A_Racer::update(float deltaTime) {
    if (auto tr = getComponent<Transform>()) {
        tr->setPosition(sf::Vector2f(logicPosition.x, logicPosition.y));
    }

    // Llama a la función de actualización de la clase base
    Actor::update(deltaTime);
}