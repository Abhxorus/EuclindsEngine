#include "window.h"

Window::Window(int width, int height, const std::string& title) {

	m_windowPtr = EngineUtilities
		::MakeUnique<sf::RenderWindow>(sf::VideoMode(width, height), title);

	if (!m_windowPtr.isNull()) {
		m_windowPtr->setFramerateLimit(60);
		MESSAGE("Window", "Window", "Window created successfully");
	}
	else {
		ERROR("Window", "Window", "Failed to create window");
	}
}

Window::~Window() {
	m_windowPtr.release();
}

void
Window::handleEvents() {
	sf::Event event;
	while (m_windowPtr->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_windowPtr->close();
		}
	}
}

bool
Window::isOpen() const {
	if (!m_windowPtr.isNull()) {
		return m_windowPtr->isOpen();
	}
	else {
		ERROR("Window", "isOpen", "Window is null");
		return false;
	}
}

void
Window::clear(const sf::Color& color) {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->clear(color);
	}
	else {
		ERROR("Window", "clear", "Window is null");
	}
}

void
Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->draw(drawable, states);
	}
	else {
		ERROR("Window", "draw", "Window is null");
	}
}

void
Window::display() {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->display();
	}
	else {
		ERROR("Window", "display", "Window is null");
	}
}

void
Window::update() {
	deltaTime = m_clock.restart();
}

void
Window::destroy() {
	m_windowPtr.release();
}