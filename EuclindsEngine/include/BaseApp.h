#pragma once
#include "Prerequisitos.h"
#include "Window.h"
#include "CShape.h"
class
	BaseApp {
public:
	BaseApp() = default;
	~BaseApp();

	// Funcion encargada de ejecutar la aplicacion en main
	int
		run();

	// Funcion de inicializacion
	bool
		init();

	// Funcion que se actualiza por frame
	void
		update();

	// Funcion de renderizado
	void
		render();

	void
		destroy();


private:
	EngineUtilities::TSharedPointer<Window> m_windowPtr;

	EngineUtilities::TSharedPointer<CShape>   m_shapePtr;
};