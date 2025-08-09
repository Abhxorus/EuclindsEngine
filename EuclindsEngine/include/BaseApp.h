#pragma once

#include "Prerequisitos.h"
#include "Window.h"
#include "CShape.h"
#include "ECS/Actor.h"
#include <vector> 

/**
 * @class BaseApp
 * @brief Base application class managing the window,
 * rendering shape, and actor lifecycle.
 *
 * This class provides a general structure for a minimal
 * engine or application loop.
 * It handles window creation, initialization of components, rendering,
 * and cleanup.
 */
	class
	BaseApp {
	public:
		/**
		 * @brief Default constructor.
		 */
		BaseApp() = default;

		/**
		 * @brief Destructor.
		 *
		 * Calls the destroy method to clean up resources if necessary.
		 */
		~BaseApp();

		/**
		 * @brief Runs the main application loop.
		 *
		 * This function controls the primary execution of the application,
		 * usually containing the loop that handles updates and rendering.
		 *
		 * @return Exit code (typically 0 if successful).
		 */
		int
			run();

		/**
		 * @brief Initializes the application, window, and resources.
		 *
		 * This method must be called before running the main loop.
		 *
		 * @return true if initialization was successful; false otherwise.
		 */
		bool
			init();

		/**
		 * @brief Updates the logic of the application.
		 *
		 * Typically called every frame within the main loop to update entities and game state.
		 */
		void
			update();

		/**
		 * @brief Renders the scene or graphical content.
		 *
		 * Called every frame to draw visual elements onto the window.
		 */
		void
			render();

		/**
		 * @brief Cleans up resources used by the application.
		 *
		 * This should be called once the application is closing to ensure memory is released.
		 */
		void
			destroy();

	private:
		/**
		 * @brief Shared pointer to the main application window.
		 */
		EngineUtilities::TSharedPointer<Window> m_windowPtr;

		/**
		 * @brief Shared pointer to a shape object used for rendering
		 * (e.g., a circle or other primitive).
		 */
		EngineUtilities::TSharedPointer<CShape> m_shapePtr;

		/**
		 * @brief Shared pointer to an Actor instance
		 * (likely representing a game object).
		 */
		EngineUtilities::TSharedPointer<Actor> m_ACircle;
		EngineUtilities::TSharedPointer<Actor> m_ATrack;

		std::vector<sf::Vector2f> m_waypoints;     ///< Lista de puntos a seguir
		size_t m_currentWaypointIndex = 0;

};