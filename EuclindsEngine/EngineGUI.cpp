#include "EngineGUI.h"
#include "window.h"
#include "ECS/Actor.h"

void
EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
	ImGui::SFML::Init(*window->m_windowPtr);
	SetupVektorCoreGUIStyle();
}

void
EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window,
	sf::Time deltaTime) {
	ImGui::SFML::Update(*window->m_windowPtr, deltaTime);
}

void
EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window) {
	ImGui::SFML::Render(*window->m_windowPtr);
}

void
EngineGUI::processEvent(const sf::Window& window, const sf::Event& event) {
	ImGui::SFML::ProcessEvent(window, event);
}

void
EngineGUI::destroy() {
	ImGui::SFML::Shutdown();
}

// Pon esto en algún archivo fuente relevante (.cpp)
void
EngineGUI::SetupVektorCoreGUIStyle() {
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 4.0f;
	style.FrameRounding = 4.0f;
	style.GrabRounding = 4.0f;
	style.ScrollbarRounding = 4.0f;
	style.TabRounding = 2.0f;
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.WindowPadding = ImVec2(10, 10);
	style.FramePadding = ImVec2(8, 4);
	style.ItemSpacing = ImVec2(8, 6);
	style.ItemInnerSpacing = ImVec2(6, 4);

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); // Blanco puro
	colors[ImGuiCol_TextDisabled] = ImVec4(0.48f, 0.48f, 0.68f, 1.00f); // Gris violáceo
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.08f, 1.00f); // Negro profundo
	colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.12f, 1.00f); // Muy oscuro
	colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.13f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.46f, 0.19f, 0.82f, 0.90f); // Morado neón
	colors[ImGuiCol_BorderShadow] = ImVec4(0.17f, 0.00f, 0.20f, 0.60f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.10f, 0.18f, 1.00f); // Morado muy oscuro
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.53f, 0.29f, 0.90f, 0.55f); // Morado brillante semi
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f); // Morado neón fuerte
	colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.10f, 0.18f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.46f, 0.19f, 0.82f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.09f, 0.09f, 0.13f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.10f, 0.18f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.10f, 0.17f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.38f, 0.16f, 0.66f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.53f, 0.29f, 0.90f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.28f, 1.00f, 1.00f); // Rosa neón
	colors[ImGuiCol_SliderGrab] = ImVec4(0.85f, 0.28f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f); // Rosa más neón
	colors[ImGuiCol_Button] = ImVec4(0.22f, 0.11f, 0.34f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.85f, 0.28f, 1.00f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.25f, 0.14f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.16f, 0.66f, 0.90f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.62f, 0.22f, 0.95f, 0.40f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.85f, 0.28f, 1.00f, 0.80f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.10f, 0.18f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.85f, 0.28f, 1.00f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.09f, 0.09f, 0.13f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.25f, 0.14f, 0.38f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.68f, 0.47f, 0.99f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.38f, 0.16f, 0.66f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.25f, 1.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.85f, 0.28f, 1.00f, 0.25f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.85f, 0.28f, 1.00f, 0.70f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.62f, 0.22f, 0.95f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.62f, 0.22f, 0.95f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.40f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.60f);

}