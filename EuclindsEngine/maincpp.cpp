#include <SFML/Graphics.hpp>

sf::RenderWindow* window;
sf::CircleShape* circle;//creamos el puntero

void
Init()
{
    window = new sf::RenderWindow(sf::VideoMode(1000, 800), "Euclinds Engine");
    circle = new sf::CircleShape(100.0f);//inicializamos el puntero
    circle->setFillColor(sf::Color::Magenta);//le damos informacion al puntero
    circle->setPosition(200.f, 150.f);
}

void
HandleEvents()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        //cerrar la ventana si el usuario lo indica
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }
    }

}

void
Update()
{
    //Aqui va la logica
}

void
Render()
{
    window->clear();
    window->draw(*circle);//dibujamos el circulo
    window->display();
}

void
Destroy()
{
    delete window;
    delete circle;//destruimos el circulo
}

int main()
{
    Init();

    while (window->isOpen())
    {
        HandleEvents();
        Update();
        Render();
    }

    Destroy();
    return 0;
}