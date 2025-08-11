/**
 * @file ActorSerializer.h
 * @brief Proporciona funciones de serialización y deserialización para actores.
 *
 * Este archivo define las funciones para guardar y cargar una colección de actores
 * desde y hacia un archivo. Esto permite la persistencia de datos de la escena.
 */

#pragma once

#include "ECS/Actor.h"
#include "Prerequisitos.h"
#include <vector>
#include <string>

 /**
  * @namespace ActorSerializer
  * @brief Un conjunto de utilidades para serializar y deserializar objetos Actor.
  *
  * Este espacio de nombres contiene funciones estáticas para guardar y cargar
  * actores desde archivos, lo que es esencial para la gestión de escenas.
  */
namespace ActorSerializer {
    /**
     * @brief Guarda una colección de actores en un archivo.
     *
     * Serializa los datos de cada actor en el vector proporcionado y los escribe
     * en un archivo especificado.
     *
     * @param filename El nombre del archivo donde se guardarán los actores.
     * @param actors Un vector constante de punteros compartidos a los actores a guardar.
     * @return `true` si la operación de guardado fue exitosa, `false` en caso contrario.
     */
    bool saveActorsToFile(const std::string& filename, const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

    /**
     * @brief Carga una colección de actores desde un archivo.
     *
     * Lee los datos de un archivo, los deserializa y crea nuevos actores,
     * añadiéndolos al vector de actores proporcionado.
     *
     * @param filename El nombre del archivo desde donde se cargarán los actores.
     * @param actors Un vector de punteros compartidos donde se añadirán los actores cargados.
     * @return `true` si la operación de carga fue exitosa, `false` en caso contrario.
     */
    bool loadActorsFromFile(const std::string& filename, std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);
}