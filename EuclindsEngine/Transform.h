#pragma once
#include "Prerequisitos.h"
#include "ECS/Component.h"
#include <SFML/System/Vector2.hpp>

/**
 * @class Transform
 * @brief Componente que representa la transformación de una entidad en el mundo (posición, rotación y escala).
 */
class Transform : public Component {
public:
    /**
     * @brief Constructor por defecto. Inicializa los valores a posición (0,0), rotación (0,0) y escala (1,1).
     */
    Transform()
        : Component(ComponentType::TRANSFORM),
        m_position(0.f, 0.f),
        m_rotation(0.f, 0.f),
        m_scale(1.f, 1.f) {}

    // Setters
    void setPosition(const sf::Vector2f& position) { m_position = position; }
    void setRotation(const sf::Vector2f& rotation) { m_rotation = rotation; }
    void setScale(const sf::Vector2f& scale) { m_scale = scale; }

    // Getters
    const sf::Vector2f& getPosition() const { return m_position; }
    const sf::Vector2f& getRotation() const { return m_rotation; }
    const sf::Vector2f& getScale() const { return m_scale; }

    // Métodos sobrescritos de Component
    void start(float deltaTime) override {}
    void update(float deltaTime) override {}
    void render(const EngineUtilities::TSharedPointer<Window>& window) override {}
    void destroy() override {}

private:
    sf::Vector2f m_position;
    sf::Vector2f m_rotation;
    sf::Vector2f m_scale;
};
