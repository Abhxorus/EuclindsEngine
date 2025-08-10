#pragma once

#include "Prerequisitos.h"
#include "Window.h"
#include "CShape.h"
#include "ECS/Actor.h"
#include <vector>
#include "EngineGUI.h"
#include "ECS/A_Racer.h"

/* new */
#include "ECS/A_Player.h"
#include "Systems/PlayerInputSystem.h"
#include "Systems/SteeringSystem.h"
#include "Systems/WaypointFollowSystem.h"
#include "Systems/RaceSystem.h"
#include "Components/RaceCountdown.h"      /* countdown (does not block Track or ImGui) */

/*
 * @class BaseApp
 * @brief Main application class. Manages window, actors, main loop, and GUI panels.
 */
class BaseApp {
public:
	BaseApp() = default;
	~BaseApp();

	/*
	 * @return Exit code (typically 0 if successful).
	 */
	int run();

	/*
	 * @return true if initialization was successful; false otherwise.
	 */
	bool init();

	/*
	 * Updates the logic of the application each frame.
	 */
	void update();

	/*
	 * Renders the scene and GUI each frame.
	 */
	void render();

	/*
	 * Cleans up resources used by the application.
	 */
	void destroy();

private:
	EngineUtilities::TSharedPointer<Window>   m_windowPtr;
	EngineUtilities::TSharedPointer<Actor>    m_ACircle;
	EngineUtilities::TSharedPointer<Actor>    m_ATrack;
	EngineUtilities::TSharedPointer<A_Racer>  m_racerNPC;

	/* Player and NPC list for systems */
	EngineUtilities::TSharedPointer<A_Player> m_player;
	std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_npcs;

	std::vector<EngineUtilities::TSharedPointer<Actor>> actorsVector; /* All actors for easy GUI access */
	EngineGUI m_engineGUI;

	std::vector<sf::Vector2f> m_waypoints; /* Waypoints for bots, player, etc. */

	/* === Systems (using TUniquePtr) === */
	EngineUtilities::TUniquePtr<PlayerInputSystem>    m_playerInputSystem;
	EngineUtilities::TUniquePtr<SteeringSystem>       m_steeringSystem;
	EngineUtilities::TUniquePtr<WaypointFollowSystem> m_waypointFollowSystem;
	EngineUtilities::TUniquePtr<RaceSystem>           m_raceSystem;

	/* === Race countdown (blocks ONLY player input and waypoint-follow until GO) === */
	RaceCountdown m_countdown{ 3.f };
	bool m_raceArmed = false;  /* countdown running */
	bool m_raceLive = false;   /* GO! reached */

	/* === Meta de carrera === */
	bool m_raceFinished = false;
	int  m_finalPlace = -1;
	int  m_lapsToWin = 3;
	bool m_npcFinished = false; /* si el NPC termina antes, se congela; NO acaba la carrera */

	/* Shared base speed for initial setup */
	float m_sharedMaxSpeed = 260.f;

	/* NPC handicap factor relative to player's max speed (1.0 = same speed) */
	float m_npcSpeedFactor = 0.95f;    /* adjust 0.90–0.98 to taste */

	/* helper */
	void resetRace();
};