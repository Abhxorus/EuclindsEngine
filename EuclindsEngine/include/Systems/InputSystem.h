#pragma once

#include "Prerequisitos.h"
#include "ECS/Player.h"

/**
 * @file PlayerInputSystem.h
 * @brief Sistema de entrada para el control del jugador.
 *
 * Define la configuración necesaria para vincular un jugador a un sistema
 * que procesa su entrada, así como la lógica para manejar diferentes modos
 * de control (Direct y TargetSeek).
 *
 * @see A_Player
 */

 /**
  * @struct PlayerInputConfig
  * @brief Configuración necesaria para inicializar un PlayerInputSystem.
  *
  * Contiene punteros al jugador y a la ventana de renderizado para poder
  * capturar y procesar eventos de entrada.
  */
struct PlayerInputConfig {
    /// Puntero compartido al jugador controlado.
    EngineUtilities::TSharedPointer<A_Player> player;

    /// Puntero a la ventana de renderizado (puede ser nulo).
    sf::RenderWindow* renderWindow = nullptr;
};

/**
 * @class PlayerInputSystem
 * @brief Sistema encargado de procesar la entrada del usuario y aplicarla a un jugador.
 *
 * Contiene lógica para ambos modos de control soportados por A_Player:
 * - Direct: Control tipo WASD.
 * - TargetSeek: Movimiento hacia un objetivo designado.
 */
class PlayerInputSystem {
public:
    /**
     * @brief Constructor.
     * @param cfg Configuración del sistema de entrada.
     */
    PlayerInputSystem(const PlayerInputConfig& cfg) : cfg(cfg) {}

    /**
     * @brief Destructor por defecto.
     */
    ~PlayerInputSystem() = default;

    /**
     * @brief Actualiza el sistema de entrada.
     * @param dt Delta de tiempo desde el último frame (segundos).
     */
    void update(float dt);

private:
    /// Configuración del sistema.
    PlayerInputConfig cfg;

    /**
     * @brief Procesa entrada en modo Direct.
     * @param dt Delta de tiempo desde el último frame (segundos).
     * @note Usado cuando el modo de control del jugador es Direct.
     */
    void updateDirect(float dt);

    /**
     * @brief Procesa entrada en modo TargetSeek.
     * @note Usado cuando el modo de control del jugador es TargetSeek.
     */
    void updateTargetSeek();
};
