#include "ECS/Actor.h"

Actor::Actor(const std::string& actorName) {
	// Setup Actor Name
	m_name = actorName;

	// Setup Shape
	EngineUtilities::TSharedPointer<CShape> shape = EngineUtilities::MakeShared<CShape>();
	addComponent(shape);

	// Setup Transform
	//EngineUtilities::TSharedPointer<Transform> transform = EngineUtilities::MakeShared<Transform>();
	//addComponent(transform);

}

void
Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {

}