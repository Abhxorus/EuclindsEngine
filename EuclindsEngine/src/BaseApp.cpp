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
        Update();
        render();
    }

    destroy();
    return 0;
}

bool BaseApp::init()
{
    m_window = new Window(1920, 1080, "Euclinds Engine");
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
    delete m_circle;//destruimos el circulo
    m_window->destroy();
}

