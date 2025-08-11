/**
 * @file EngineGUI.cpp
 * @brief Implementación de la clase EngineGUI para la interfaz gráfica del motor.
 *
 * Este archivo maneja la inicialización, actualización, renderizado y destrucción de la interfaz de usuario
 * basada en ImGui. También define los paneles principales como el menú, la jerarquía, el inspector,
 * la consola y el gestor de archivos.
 */

#include "EngineGUI.h"
#include "Window.h"
#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include <filesystem>
#include "ActorSerializer.h"

namespace fs = std::filesystem;


/**
 * @brief Inicializa la interfaz de usuario ImGui.
 *
 * Se encarga de iniciar el contexto de ImGui para la ventana de SFML y
 * establece el estilo visual personalizado del motor.
 *
 * @param window Puntero a la ventana principal del motor.
 */
void EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
    ImGui::SFML::Init(*window->m_windowPtr);
    SetupVektorCoreGUIStyle();
    selectedActorIndex = -1;
}

/**
 * @brief Actualiza la interfaz de usuario.
 *
 * Llama a la función de actualización de ImGui para procesar la lógica de la UI
 * en cada fotograma.
 *
 * @param window Puntero a la ventana principal.
 * @param deltaTime El tiempo transcurrido desde el último fotograma.
 */
void EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window, sf::Time deltaTime) {
    ImGui::SFML::Update(*window->m_windowPtr, deltaTime);
}

/**
 * @brief Renderiza la interfaz de usuario.
 *
 * Dibuja todos los elementos de la interfaz en la ventana.
 *
 * @param window Puntero a la ventana principal.
 */
void EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window) {
    ImGui::SFML::Render(*window->m_windowPtr);
}

/**
 * @brief Destruye el contexto de la interfaz de usuario.
 *
 * Cierra ImGui de forma segura, liberando los recursos asociados.
 */
void EngineGUI::destroy() {
    ImGui::SFML::Shutdown();
}

/**
 * @brief Procesa los eventos de entrada para la interfaz de usuario.
 *
 * Permite que ImGui maneje eventos como clics de ratón, teclado, etc.
 *
 * @param window La ventana de SFML donde se muestran los eventos.
 * @param event El evento de entrada a procesar.
 */
void EngineGUI::processEvent(const sf::Window& window, const sf::Event& event) {
    ImGui::SFML::ProcessEvent(window, event);
}


/**
 * @brief Configura un estilo visual personalizado para la interfaz de usuario ImGui.
 *
 * Define la apariencia de las ventanas, botones, barras de desplazamiento, etc.,
 * utilizando una paleta de colores oscuros similar a la de un motor de juego.
 */
void EngineGUI::SetupVektorCoreGUIStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Estilo de bordes y esquinas
    style.WindowRounding = 2.0f;
    style.FrameRounding = 2.0f;
    style.GrabRounding = 2.0f;
    style.ScrollbarRounding = 2.0f;
    style.TabRounding = 2.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;

    // Espaciado
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);

    ImVec4* colors = style.Colors;

    // Colores de fondo
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.85f);

    // Colores de texto
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    // Colores de bordes
    colors[ImGuiCol_Border] = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Colores de frames
    colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);

    // Colores de botones
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.27f, 0.30f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.32f, 0.32f, 0.36f, 1.00f);

    // Colores de títulos
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);

    // Color de barra de menú
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);

    // Colores de scrollbars
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.44f, 1.00f);

    // Colores de elementos de selección
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.79f, 1.00f, 1.00f);

    // Colores de headers (listas, colapsables)
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.27f, 0.27f, 0.30f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.32f, 0.32f, 0.36f, 1.00f);

    // Colores de tabs
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.28f, 0.31f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);

    // Colores de resaltado
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
}

/*
 * @brief Dibuja la barra de menú principal.
 *
 * Incluye los menús "File", "Edit", "View", "Window" y "Help",
 * con opciones básicas como guardar, cargar, editar, etc.
 */
    void EngineGUI::menuBar() {
    if (ImGui::BeginMainMenuBar()) {
        /* ==================== FILE ==================== */
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) { /* TODO */ }
            if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) { /* TODO */ }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { /* TODO */ }
            if (ImGui::MenuItem("Save Scene As…")) { /* TODO */ }
            ImGui::Separator();

            /* ---------- Sub-menú con los archivos recientes ---------- */
            if (ImGui::BeginMenu("Recent Files")) {
                const char* recents[] = { "level01.scene", "tutorial.scene", "myTest.scene" };
                for (const char* f : recents) {
                    if (ImGui::MenuItem(f)) { /* TODO: abrir f */ }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) { /* TODO: salir */ }
            ImGui::EndMenu();
        }

        /* ==================== EDIT ==================== */
        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Undo", "Ctrl+Z");
            ImGui::MenuItem("Redo", "Ctrl+Y");
            ImGui::Separator();
            ImGui::MenuItem("Cut", "Ctrl+X");
            ImGui::MenuItem("Copy", "Ctrl+C");
            ImGui::MenuItem("Paste", "Ctrl+V");
            ImGui::EndMenu();
        }

        /* ==================== VIEW ==================== */
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Hierarchy");
            ImGui::MenuItem("Inspector");
            ImGui::MenuItem("Console");
            ImGui::MenuItem("File Manager");
            ImGui::EndMenu();
        }

        /* ==================== WINDOW ==================== */
        if (ImGui::BeginMenu("Window")) {
            ImGui::MenuItem("Maximize");
            ImGui::MenuItem("Minimize");
            ImGui::MenuItem("Reset Layout");
            ImGui::EndMenu();
        }

        /* ==================== HELP ==================== */
        if (ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("Documentation (F1)");
            ImGui::MenuItem("Report a Bug");
            ImGui::MenuItem("About EuclindsEngine");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

/**
 * @brief Dibuja el panel de Jerarquía de la escena.
 *
 * Muestra una lista de todos los actores en la escena. Permite seleccionar un actor,
 * resaltándolo con un borde morado.
 *
 * @param actors Un vector de punteros compartidos a los actores de la escena.
 */
void EngineGUI::hierarchy(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
    ImGui::Begin("Hierarchy");
    for (int i = 0; i < actors.size(); ++i) {
        std::string name = actors[i]->getName();
        bool isSelected = (selectedActorIndex == i);

        if (ImGui::Selectable(name.c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick)) {
            selectedActorIndex = i;
        }

        if (isSelected) {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 p_min = ImGui::GetItemRectMin();
            ImVec2 p_max = ImGui::GetItemRectMax();
            draw_list->AddRect(p_min, p_max, IM_COL32(180, 40, 255, 255), 4.0f, 0, 3.5f);
        }
    }
    ImGui::End();
}

/**
 * @brief Dibuja el panel de Inspector para el actor seleccionado.
 *
 * Permite visualizar y modificar los componentes del actor seleccionado, como su
 * posición, rotación y escala.
 *
 * @param actors Un vector de punteros compartidos a los actores de la escena.
 */
void EngineGUI::inspector(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
    ImGui::Begin("Inspector");

    if (selectedActorIndex >= 0 && selectedActorIndex < actors.size() && actors[selectedActorIndex]) {
        auto actor = actors[selectedActorIndex];

        // --- Bloque de información básica ---
        bool isStatic = false;
        ImGui::Checkbox("##Static", &isStatic);
        ImGui::SameLine();

        char objectName[128];
        std::string name = actor->getName();
        std::copy(name.begin(), name.end(), objectName);
        objectName[name.size()] = '\0';

        ImGui::InputText("##ObjectName", objectName, IM_ARRAYSIZE(objectName));
        ImGui::SameLine();

        if (ImGui::Button("Icon")) {
            // Lógica del botón de icono aquí
        }

        ImGui::Separator();

        const char* tags[] = { "Untagged", "Player", "Enemy", "Environment" };
        static int currentTag = 0;
        ImGui::Combo("Tag", &currentTag, tags, IM_ARRAYSIZE(tags));
        ImGui::SameLine();

        const char* layers[] = { "Default", "TransparentFX", "Ignore Raycast", "Water", "UI" };
        static int currentLayer = 0;
        ImGui::Combo("Layer", &currentLayer, layers, IM_ARRAYSIZE(layers));
        ImGui::Separator();
        // --- Fin bloque de información básica ---

        auto transform = actor->getComponent<Transform>();
        if (transform) {
            float pos[3] = { transform->getPosition().x, transform->getPosition().y, 0.0f };
            float rot[3] = { transform->getRotation().x, transform->getRotation().y, 0.0f };
            float sca[3] = { transform->getScale().x, transform->getScale().y, 1.0f };

            ImGui::PushItemWidth(60);

            // Position
            ImGui::Text("Position");
            ImGui::SameLine(100);
            ImGui::PushID("PosX");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.25f, 0.25f, 1.0f));
            ImGui::Button("X");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::DragFloat("##PosX", &pos[0], 0.5f);
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("PosY");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.85f, 0.25f, 1.0f));
            ImGui::Button("Y");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::DragFloat("##PosY", &pos[1], 0.5f);
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("PosZ");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.85f, 1.0f));
            ImGui::Button("Z");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::DragFloat("##PosZ", &pos[2], 0.5f);
            ImGui::PopID();

            // Rotation
            ImGui::Text("Rotation");
            ImGui::SameLine(100);
            ImGui::PushID("RotX");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.25f, 0.25f, 1.0f));
            ImGui::Button("X");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::DragFloat("##RotX", &rot[0], 1.0f);
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("RotY");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.85f, 0.25f, 1.0f));
            ImGui::Button("Y");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::DragFloat("##RotY", &rot[1], 1.0f);
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("RotZ");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.85f, 1.0f));
            ImGui::Button("Z");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::DragFloat("##RotZ", &rot[2], 1.0f);
            ImGui::PopID();

            // Scale
            ImGui::Text("Scale");
            ImGui::SameLine(100);
            ImGui::PushID("ScaX");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.25f, 0.25f, 1.0f));
            ImGui::Button("X");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::DragFloat("##ScaX", &sca[0], 0.05f, 0.01f, 100.0f);
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("ScaY");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.85f, 0.25f, 1.0f));
            ImGui::Button("Y");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::DragFloat("##ScaY", &sca[1], 0.05f, 0.01f, 100.0f);
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("ScaZ");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.85f, 1.0f));
            ImGui::Button("Z");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::DragFloat("##ScaZ", &sca[2], 0.05f, 0.01f, 100.0f);
            ImGui::PopID();

            // Aplicar cambios
            transform->setPosition(sf::Vector2f(pos[0], pos[1]));
            transform->setRotation(sf::Vector2f(rot[0], rot[1]));
            transform->setScale(sf::Vector2f(sca[0], sca[1]));

            ImGui::PopItemWidth();
        }
    }
    ImGui::End();
}

/**
 * @brief Dibuja el panel de la Consola.
 *
 * Simplemente muestra un texto de marcador de posición para futuros mensajes de log.
 */
void EngineGUI::console() {
    ImGui::Begin("Console");
    ImGui::Text("Logs and messages will appear here.");
    ImGui::End();
}


/**
 * @brief Dibuja un contorno de color alrededor del actor seleccionado.
 *
 * Utiliza un color púrpura neón para resaltar el objeto seleccionado en la ventana de renderizado.
 *
 * @param renderWindow Puntero a la ventana de renderizado de SFML.
 * @param actors Un vector de punteros compartidos a los actores de la escena.
 */
void EngineGUI::drawSelectedOutline(sf::RenderWindow* renderWindow, const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
    int idx = selectedActorIndex;

    if (idx >= 0 && idx < actors.size() && actors[idx]) {
        auto shapeComp = actors[idx]->getComponent<CShape>();

        if (shapeComp) {
            auto shapePtr = shapeComp->getShapePtr();
            if (shapePtr) {
                float thickness = 2.0f;
                if (actors[idx]->getName() == "Track Actor") {
                    thickness = 0.5f;
                }

                sf::Color oldOutlineColor = shapePtr->getOutlineColor();
                float oldThickness = shapePtr->getOutlineThickness();
                sf::Color oldFill = shapePtr->getFillColor();

                shapePtr->setOutlineThickness(thickness);
                shapePtr->setOutlineColor(sf::Color(180, 40, 255));
                shapePtr->setFillColor(sf::Color(0, 0, 0, 0));
                renderWindow->draw(*shapePtr);

                // Restaurar el estado original del shape
                shapePtr->setOutlineColor(oldOutlineColor);
                shapePtr->setOutlineThickness(oldThickness);
                shapePtr->setFillColor(oldFill);
            }
        }
    }
}


/**
 * @brief Dibuja el panel del Gestor de Archivos.
 *
 * Permite al usuario guardar y cargar la lista de actores en un archivo,
 * y muestra una lista de archivos disponibles en el directorio actual.
 *
 * @param actors Un vector de punteros compartidos a los actores, que puede ser modificado al cargar.
 */
void EngineGUI::fileManagerPanel(std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
    ImGui::Begin("File Manager");
    static char fileName[128] = "actors.txt";
    ImGui::InputText("Actors File", fileName, IM_ARRAYSIZE(fileName));

    if (ImGui::Button("Save Actors")) {
        ActorSerializer::saveActorsToFile(fileName, actors);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Actors")) {
        actors.clear();
        ActorSerializer::loadActorsFromFile(fileName, actors);
    }

    ImGui::Separator();
    ImGui::Text("Available files:");

    for (const auto& entry : fs::directory_iterator(".")) {
        std::string fname = entry.path().filename().string();
        if (fname.size() >= 4 && fname.substr(fname.size() - 4) == ".txt") {
            if (ImGui::Selectable(fname.c_str())) {
                strncpy(fileName, fname.c_str(), sizeof(fileName));
            }
        }
    }

    ImGui::End();
}