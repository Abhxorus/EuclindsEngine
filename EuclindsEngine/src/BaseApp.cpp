#include "BaseApp.h"

BaseApp::~BaseApp() {
}

int
BaseApp::run() {
	if (!init()) {
		ERROR("BaseApp",
			"run",
			"Initializes result on a false statemente, check method validations");
	}

	while (m_windowPtr->isOpen()) {
		m_windowPtr->handleEvents();
		update();
		render();
	}

	destroy();
	return 0;
}

bool
BaseApp::init() {
	m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "Labrid Engine");
	if (!m_windowPtr) {
		ERROR("BaseApp",
			"init",
			"Failed to create window pointer, check memory allocation");
		return false;
	}

	// Create a circle shape
	m_shapePtr = EngineUtilities::MakeShared<CShape>();
	if (m_shapePtr)
	{
		m_shapePtr->createShape(ShapeType::CIRCLE);
		m_shapePtr->setFillColor(sf::Color::Green);
		m_shapePtr->setPosition(200.f, 150.f);
	}
	return true;
}

void
BaseApp::update() {
}

void
BaseApp::render() {
	m_windowPtr->clear();
	if (m_shapePtr) {
		m_shapePtr->render(m_windowPtr);
	}
	m_windowPtr->display();
}

void
BaseApp::destroy() {
	//m_shapePtr.reset();
	//m_window->destroy();
}