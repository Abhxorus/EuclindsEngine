/**
 * @file Actor.cpp
 * @brief Implementación de la clase Actor.
 *
 * Esta clase sirve como la entidad base en un sistema de Componentes de Entidad (ECS).
 * Los actores son contenedores para diferentes componentes, como Transform y CShape,
 * que definen su comportamiento y apariencia.
 */
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "CShape.h"

 /**
  * @brief Constructor de la clase Actor.
  *
  * Inicializa el actor con un nombre y le adjunta componentes esenciales:
  * un componente de forma (`CShape`) y un componente de transformación (`Transform`).
  *
  * @param actorName El nombre único que se le asignará al actor.
  */
Actor::Actor(const std::string& actorName) {
    // Configurar el nombre del actor
    m_name = actorName;

    // Configurar el componente de forma (CShape)
    EngineUtilities::TSharedPointer<CShape> shape = EngineUtilities::MakeShared<CShape>();
    addComponent(shape);

    // Configurar el componente de transformación (Transform)
    EngineUtilities::TSharedPointer<Transform> transform = EngineUtilities::MakeShared<Transform>();
    addComponent(transform);
}

/**
 * @brief Renderiza el actor y sus componentes en la ventana.
 *
 * Itera sobre todos los componentes del actor y llama a su método `render`,
 * dibujando la representación visual del actor.
 *
 * @param window Puntero a la ventana donde se renderizará el actor.
 */
void Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {
    for (unsigned int i = 0; i < components.size(); i++) {
        auto component = components[i];
        if (component) {
            component->render(window);
        }
    }
}

/**
 * @brief Inicia la lógica del actor.
 *
 * Este método está destinado a la inicialización de la lógica del juego. Actualmente
 * está vacío, pero puede ser sobrecargado en clases derivadas.
 */
void Actor::start() {
    // Implementación de lógica de inicio
}

/**
 * @brief Actualiza la lógica del actor en cada fotograma.
 *
 * Sincroniza la posición, rotación y escala del componente de forma (`CShape`)
 * con la del componente de transformación (`Transform`).
 *
 * @param deltaTime El tiempo transcurrido desde el último fotograma.
 */
void Actor::update(float deltaTime) {
    auto transform = getComponent<Transform>();
    auto shape = getComponent<CShape>();

    if (transform && shape) {
        shape->setPosition(transform->getPosition());
        shape->setRotation(transform->getRotation().x);
        shape->setScale(transform->getScale());
    }
}

/**
 * @brief Destruye los recursos del actor.
 *
 * Este método está destinado a la limpieza de recursos. Actualmente
 * está vacío, pero puede ser sobrecargado en clases derivadas.
 */
void Actor::destroy() {
    // Implementación de la lógica de destrucción
}

/**
 * @brief Asigna una textura al componente de forma del actor.
 *
 * Busca el componente `CShape` y, si existe y la textura es válida,
 * la establece y la añade a la lista de componentes.
 *
 * @param texture Puntero compartido a la textura que se va a asignar.
 */
void Actor::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
    auto shape = getComponent<CShape>();
    if (shape) {
        if (!texture.isNull()) {
            shape->setTexture(texture);
            addComponent(texture);
        }
    }
}