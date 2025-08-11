/**
 * @file ResourceManager.h
 * @brief Define la clase ResourceManager para la gestión de recursos del motor.
 *
 * Esta clase implementa el patrón de diseño Singleton para asegurar una única
 * instancia que maneja la carga y acceso a los recursos, como las texturas.
 * Previene la duplicación de carga y optimiza el uso de la memoria.
 */

#pragma once

#include "Prerequisitos.h"
#include "ECS/Texture.h"
#include <unordered_map>

 /**
  * @class ResourceManager
  * @brief Clase Singleton que administra y cachea los recursos del motor.
  *
  * Se encarga de cargar, almacenar y proporcionar acceso a diferentes tipos
  * de recursos, como texturas, de manera eficiente. Su diseño Singleton
  * garantiza que todos los componentes del motor compartan la misma instancia
  * de recursos.
  */
class ResourceManager {
private:
    /**
     * @brief Constructor privado para implementar el patrón Singleton.
     * No se puede instanciar desde fuera de la clase.
     */
    ResourceManager() = default;

    /**
     * @brief Prohibir el constructor de copia.
     */
    ResourceManager(const ResourceManager&) = delete;

    /**
     * @brief Prohibir el operador de asignación.
     */
    ResourceManager& operator=(const ResourceManager&) = delete;

public:
    /**
     * @brief Proporciona la única instancia de la clase ResourceManager.
     * @return Una referencia a la única instancia de ResourceManager.
     */
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    /**
     * @brief Carga una textura desde un archivo y la almacena en el caché.
     *
     * @param filename El nombre base del archivo de la textura (sin la extensión).
     * @param extension La extensión del archivo, por ejemplo, "png" o "jpg".
     * @return `true` si la textura se cargó correctamente, `false` en caso contrario.
     */
    bool loadTexture(const std::string& filename, const std::string& extension);

    /**
     * @brief Obtiene una textura del caché.
     *
     * Si la textura no ha sido cargada previamente, este método devolverá un
     * puntero nulo. Se debe llamar a `loadTexture` antes de intentar obtenerla.
     *
     * @param fileName El nombre completo del archivo de la textura (incluyendo la extensión).
     * @return Un puntero compartido a la textura, o un puntero nulo si no se encuentra.
     */
    EngineUtilities::TSharedPointer<Texture> getTexture(const std::string& fileName);

private:
    /**
     * @brief Mapa que almacena las texturas cargadas.
     *
     * La clave es el nombre completo del archivo (ej. "player.png") y el valor
     * es un puntero compartido a la instancia de la textura.
     */
    std::unordered_map<std::string, EngineUtilities::TSharedPointer<Texture>> m_textures;
};