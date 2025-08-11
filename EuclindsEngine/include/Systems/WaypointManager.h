/**
 * @file WaypointManager.h
 * @brief Define la clase WaypointManager para gestionar puntos de ruta.
 *
 * Esta clase almacena y proporciona acceso a una serie de puntos de ruta
 * (waypoints) que pueden ser utilizados para la navegación o el movimiento
 * de entidades en el motor.
 */

#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

 /**
  * @class WaypointManager
  * @brief Administra una colección de puntos de ruta en un entorno 2D.
  *
  * Esta clase es responsable de guardar y recuperar una lista ordenada de
  * coordenadas 2D que definen una ruta.
  */
class WaypointManager {
public:
    /**
     * @brief Establece los puntos de ruta para el manager.
     * @param pts Un vector de objetos sf::Vector2f que representan los puntos de ruta.
     */
    void setWaypoints(const std::vector<sf::Vector2f>& pts) {
        waypoints_ = pts;
    }

    /**
     * @brief Obtiene una referencia constante a los puntos de ruta.
     * @return Una referencia constante al vector de puntos de ruta.
     */
    const std::vector<sf::Vector2f>& waypoints() const {
        return waypoints_;
    }

private:
    /**
     * @brief Vector que almacena los puntos de ruta.
     */
    std::vector<sf::Vector2f> waypoints_;
};