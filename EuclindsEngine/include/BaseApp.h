#pragma once

#include "Prerequisitos.h"
#include "window.h"

class BaseApp
{
public:
	BaseApp() = default;
	~BaseApp();

	//Funcion encargada de ejecutar la app en main
	int run();

	// Fucion de inicialización
	bool init();

	//Funcion de renderizado
	void render();

	void destroy();

	void Update();

private:
	Window* m_window;
	//sf::RenderWindow* m_window;
	sf::CircleShape* m_circle;//creamos el puntero
};
