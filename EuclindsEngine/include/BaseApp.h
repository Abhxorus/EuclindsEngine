/**
 * @file BaseApp.h
 * @brief Define la clase principal de la aplicación, BaseApp.
 *
 * BaseApp es la clase central del motor. Se encarga de la gestión de la ventana,
 * la creación y administración de actores, el bucle principal de la aplicación,
 * la integración de la GUI y la coordinación de todos los sistemas del juego.
 */

#pragma once

#include "Prerequisitos.h"
#include "Window.h"
#include "CShape.h"
#include "ECS/Actor.h"
#include <vector>
#include "EngineGUI.h"
#include "ECS/A_Racer.h"

 /* Componentes y sistemas */
#include "ECS/A_Player.h"
#include "Systems/PlayerInputSystem.h"
#include "Systems/SteeringSystem.h"
#include "Systems/WaypointFollowSystem.h"
#include "Systems/RaceSystem.h"
#include "Components/RaceCountdown.h"

/**
 * @class BaseApp
 * @brief Clase principal de la aplicación.
 *
 * Gestiona la ventana, los actores, el bucle principal, los paneles de la GUI
 * y los sistemas de juego (entrada, dirección, carrera, etc.). Es el punto
 * de orquestación de todo el motor.
 */
class BaseApp {
public:
    /**
     * @brief Constructor por defecto.
     */
    BaseApp() = default;

    /**
     * @brief Destructor de la clase.
     */
    ~BaseApp();

    /**
     * @brief Inicia el bucle principal de la aplicación.
     * @return Código de salida (normalmente 0 si es exitoso).
     */
    int run();

    /**
     * @brief Inicializa la aplicación y sus componentes.
     * @return `true` si la inicialización fue exitosa, `false` en caso contrario.
     */
    bool init();

    /**
     * @brief Actualiza la lógica de la aplicación en cada fotograma.
     */
    void update();

    /**
     * @brief Renderiza la escena y la interfaz gráfica de usuario en cada fotograma.
     */
    void render();

    /**
     * @brief Limpia y libera los recursos utilizados por la aplicación.
     */
    void destroy();

private:
    /// Puntero a la ventana principal del motor.
    EngineUtilities::TSharedPointer<Window> m_windowPtr;

    /// Punteros a los actores principales.
    EngineUtilities::TSharedPointer<Actor> m_ACircle;
    EngineUtilities::TSharedPointer<Actor> m_ATrack;
    EngineUtilities::TSharedPointer<A_Racer> m_racerNPC;

    /// Puntero al jugador y lista de NPCs.
    EngineUtilities::TSharedPointer<A_Player> m_player;
    std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_npcs;

    /// Vector de todos los actores para un fácil acceso por la GUI.
    std::vector<EngineUtilities::TSharedPointer<Actor>> actorsVector;

    /// Instancia del sistema de GUI.
    EngineGUI m_engineGUI;

    /// Puntos de ruta (waypoints) para los corredores.
    std::vector<sf::Vector2f> m_waypoints;

    /* === Sistemas (utilizando TUniquePtr) === */
    EngineUtilities::TUniquePtr<PlayerInputSystem> m_playerInputSystem;
    EngineUtilities::TUniquePtr<SteeringSystem> m_steeringSystem;
    EngineUtilities::TUniquePtr<WaypointFollowSystem> m_waypointFollowSystem;
    EngineUtilities::TUniquePtr<RaceSystem> m_raceSystem;

    /* === Cuenta atrás de la carrera === */
    /// Componente de cuenta atrás antes de que comience la carrera.
    RaceCountdown m_countdown{ 3.f };
    /// Bandera que indica si la cuenta atrás está en curso.
    bool m_raceArmed = false;
    /// Bandera que indica si la carrera ha comenzado.
    bool m_raceLive = false;

    /* === Meta de carrera === */
    /// Bandera que indica si la carrera ha terminado.
    bool m_raceFinished = false;
    /// El lugar final del jugador en la carrera.
    int m_finalPlace = -1;
    /// Número de vueltas necesarias para ganar.
    int m_lapsToWin = 3;
    /// Bandera para congelar la lógica del NPC cuando termina.
    bool m_npcFinished = false;

    /// Velocidad base compartida para la configuración inicial de los corredores.
    float m_sharedMaxSpeed = 260.f;

    /// Factor de hándicap para la velocidad del NPC en relación con el jugador (1.0 = misma velocidad).
    float m_npcSpeedFactor = 0.95f;

    /* === Funciones de ayuda === */
    /**
     * @brief Restablece el estado de la carrera a su valor inicial.
     */
    void resetRace();
};