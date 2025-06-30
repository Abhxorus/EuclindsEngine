#pragma once
#include "Prerequisitos.h"
#include "ECS\Component.h"

class Window;

class 
CShape : public Component {
public:
	CShape() = default;

	CShape(ShapeType shapeType) :	
    m_shapePtr(nullptr), 
	m_shapeType(ShapeType::EMPTY), 
    Component(ComponentType::SHAPE) { }

	virtual
	~CShape() = default;

    void
    createShape(ShapeType shapeType);

    void 
    start() override;

    void
    update(float deltaTime)override;

    void
    render(const EngineUtilities::TSharedPointer<Window>& window)override;

    void 
    destroy() override;

    void 
    setPosition(float x, float y);

    void 
    setPosition(const sf::Vector2f& position);

    void 
    setFillColor(const sf::Color& color);

    void 
    setRotation(float angle);
  
    void 
    setScale(const sf::Vector2f& scl);

private:
	EngineUtilities::TSharedPointer<sf::Shape> m_shapePtr;
	ShapeType m_shapeType;
	sf::VertexArray* m_line;
};