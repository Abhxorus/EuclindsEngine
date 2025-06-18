#pragma once
#include "Prerequisitos.h"

class Window;

class
	CShape {
public:
	CShape() = default;

	CShape(ShapeType shapeType) : m_shapePtr(nullptr), m_shapeType(ShapeType::EMPTY) { }

	~CShape() = default;

	/**
		* @Brief: This method is in charge of creating a shape based on the type provided.
		* @Param: shapeType: The type of shape to create.
	  */
	void
		createShape(ShapeType shapeType);

	void
		update(float deltaTime);

	void
		render(const EngineUtilities::TSharedPointer<Window>& window);

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
	//sf::Shape* m_shape;
	ShapeType m_shapeType;
	sf::VertexArray* m_line;
};