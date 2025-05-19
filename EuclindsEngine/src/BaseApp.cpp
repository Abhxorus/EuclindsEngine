#include "BaseApp.h"

BaseApp::~BaseApp()
{
}

int BaseApp::run()
{
    if (!init())
    {
        ERROR("BaseApp",
            "run",
            "Initializes result on a false statemente, check method validations");
    }

    while (m_window->isOpen())
    {
        HandleEvents();
        Update();
        render();
    }

    destroy();
    return 0;
}

bool BaseApp::init()
{
    m_window = new sf::RenderWindow(sf::VideoMode(1000, 800), "Euclinds Engine");
    m_circle = new sf::CircleShape(100.0f);//inicializamos el puntero
    m_circle->setFillColor(sf::Color::Magenta);//le damos informacion al puntero
    m_circle->setPosition(200.f, 150.f);
	return true;
}

void BaseApp::Update()
{
}

void BaseApp::render()
{
    m_window->clear();
    m_window->draw(*m_circle);//dibujamos el circulo
    m_window->display();
}

void BaseApp::destroy()
{
    delete m_window;
    delete m_circle;//destruimos el circulo
}

void BaseApp::HandleEvents()
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