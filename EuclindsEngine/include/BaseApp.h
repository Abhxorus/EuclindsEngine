#pragma once

#include "Prerequisitos.h"

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

	void HandleEvents();

	void Update();

private:
	sf::RenderWindow* m_window;
	sf::CircleShape* m_circle;//creamos el puntero
};
