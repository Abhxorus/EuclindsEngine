#include "window.h"

Window::Window(int width, int height, const std::string& title) {

	m_windowPtr = EngineUtilities::MakeUnique<sf::RenderWindow>(
		sf::VideoMode({ static_cast<unsigned int>(width),
										static_cast<unsigned int>(height) }),
		title,
		sf::Style::Default
	);

	if (!m_windowPtr.isNull()) {
		m_windowPtr->setFramerateLimit(60);
		MESSAGE("Window", "Window", "Window created successfully");
	}
	else {
		ERROR("Window", "Window", "Failed to create window");
	}

	ImGui::SFML::Init(*m_windowPtr);
}

Window::~Window() {
	m_windowPtr.release();
	ImGui::SFML::Shutdown();
}

void
Window::handleEvents() {

	while (const std::optional event = m_windowPtr->pollEvent())
	{
		ImGui::SFML::ProcessEvent(*m_windowPtr, *event);
		// Close window: exit
		if (event->is<sf::Event::Closed>())
			m_windowPtr->close();
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

	ImGui::SFML::Update(*m_windowPtr, deltaTime);
}

void
Window::render() {
	ImGui::SFML::Render(*m_windowPtr);
}

void
Window::destroy() {
	m_windowPtr.release();
	ImGui::SFML::Shutdown();
}