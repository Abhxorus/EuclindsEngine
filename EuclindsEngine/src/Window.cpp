#include "window.h"

Window::Window(int width, int height, const std::string& title)
{
    // Inicializar la ventana
    m_window = new sf::RenderWindow(sf::VideoMode(width, height), title);

    if (m_window)
    {
        m_window->setFramerateLimit(60); //Limitadr a 60 FPS
        MESSAGE("window", "window", "window created successfully");
    }
    else
    {
        ERROR("window", "window", "Failed to create window");
    }
}

Window::~Window()
{
    SAFE_PTR_RELEASE(m_window);
}

void Window::HandleEvents()
{
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        //cerrar la ventana si el usuario lo indica
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
        }
    }
}

bool Window::isOpen() const
{
    //Check that window is not null
    if (m_window)
    {
        m_window->isOpen();
    }
    else
    {
        ERROR("Window", "isOpen", "Window is null");
        return false;
    }
}

void Window::clear(const sf::Color& color)
{
    if (m_window)
    {
        m_window->clear(color);
    }
    else
    {
        ERROR("Window", "clear", "Window is null");
    }
}

void Window::draw(const sf::Drawable& drawble, const sf::RenderStates& states)
{
    if (m_window)
    {
        m_window->draw(drawble, states);
    }
    else
    {
        ERROR("window", "draw", "Window is null");
    }
}

void Window::display()
{
    if (m_window)
    {
        m_window->display();
    }
    else
    {
        ERROR("Window", "display", "Window is null");
    }
}

void Window::destroy()
{
    SAFE_PTR_RELEASE(m_window);
}