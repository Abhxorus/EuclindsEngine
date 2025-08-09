#include "BaseApp.h"
#include "ResourceManager.h"
#include <cmath>  // Para std::sqrt y std::pow
#include <imgui.h>          // ImGui core
#include <imgui-SFML.h>     // ImGui + SFML bridge

BaseApp::~BaseApp() {
}

int BaseApp::run() {
    if (!init()) {
        ERROR("BaseApp", "run", "Initializes result on a false statement, check method validations");
        return -1;
    }

    while (m_windowPtr->isOpen()) {
        m_windowPtr->handleEvents();
        update();
        render();
    }

    destroy();
    return 0;
}

bool BaseApp::init() {
    ResourceManager& resourceMan = ResourceManager::getInstance();

    m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "EuclidsEngine");
    if (!m_windowPtr) {
        ERROR("BaseApp", "init", "Failed to create window pointer, check memory allocation");
        return false;
    }

    // Crear Actor círculo
    m_ACircle = EngineUtilities::MakeShared<Actor>("Circle Actor");
    if (m_ACircle) {
        m_ACircle->getComponent<CShape>()->createShape(CIRCLE);
        m_ACircle->getComponent<CShape>()->setFillColor(sf::Color::White);
        m_ACircle->getComponent<Transform>()->setPosition(sf::Vector2f(899.f, 855.f));
        m_ACircle->getComponent<Transform>()->setScale(sf::Vector2f(2.f, 2.f));

        if (!resourceMan.loadTexture("sprites/Yoshi", "png")) {
            MESSAGE("BaseApp", "Init", "Can't load the texture");
        }
        m_ACircle->setTexture(resourceMan.getTexture("sprites/Yoshi"));
    }

    // Waypoints
    m_waypoints = {
        {1320.f, 855.f},
        {1320.f, 600.f},
        {1100.f, 400.f},
        {750.f, 350.f},
        {600.f, 470.f},
        {550.f, 700.f},
        {750.f, 850.f},
        {950.f, 950.f},
        {1320.f, 855.f}
    };
    m_currentWaypointIndex = 0;

    // Crear pista
    m_ATrack = EngineUtilities::MakeShared<Actor>("Track Actor");
    if (m_ATrack) {
        m_ATrack->getComponent<CShape>()->createShape(RECTANGLE);
        m_ATrack->getComponent<CShape>()->setFillColor(sf::Color::White);
        m_ATrack->getComponent<Transform>()->setPosition(sf::Vector2f(450.f, 0.f));
        m_ATrack->getComponent<Transform>()->setScale(sf::Vector2f(10.f, 19.5f));

        if (!resourceMan.loadTexture("sprites/pista", "png")) {
            MESSAGE("BaseApp", "Init", "Can't load the texture");
        }
        m_ATrack->setTexture(resourceMan.getTexture("sprites/pista"));
    }

    return true;
}

void BaseApp::update() {
    if (!m_windowPtr.isNull()) {
        m_windowPtr->update();  // Incluye ImGui::SFML::Update()
    }

    if (!m_ACircle.isNull()) {
        m_ACircle->update(m_windowPtr->deltaTime.asSeconds());

        if (m_currentWaypointIndex < m_waypoints.size()) {
            sf::Vector2f targetPos = m_waypoints[m_currentWaypointIndex];
            auto transform = m_ACircle->getComponent<Transform>();
            auto currentPos = transform->getPosition();

            float distanceToTarget = std::sqrt(
                std::pow(targetPos.x - currentPos.x, 2) +
                std::pow(targetPos.y - currentPos.y, 2)
            );

            if (distanceToTarget < 10.0f) {
                m_currentWaypointIndex++;
            }
            else {
                transform->seek(targetPos, 100.f, m_windowPtr->deltaTime.asSeconds(), 10.f);
            }
        }
    }

    if (!m_ATrack.isNull()) {
        m_ATrack->update(m_windowPtr->deltaTime.asSeconds());
    }
}

void BaseApp::render() {
    if (!m_windowPtr) return;

    m_windowPtr->clear();

    if (m_shapePtr) {
        m_shapePtr->render(m_windowPtr);
    }

    if (m_ATrack) {
        m_ATrack->getComponent<CShape>()->render(m_windowPtr);
    }

    if (m_ACircle) {
        m_ACircle->getComponent<CShape>()->render(m_windowPtr);
    }

    // 🟣 ImGui Panel de Debug
    if (!m_ACircle.isNull()) {
        auto transform = m_ACircle->getComponent<Transform>();
        sf::Vector2f& position = transform->getPosition();
        sf::Vector2f& scale = transform->getScale();

        ImGui::Begin("Debug - Yoshi");
        ImGui::Text("Posición actual:");
        ImGui::SliderFloat("X", &position.x, 0.0f, 1920.0f);
        ImGui::SliderFloat("Y", &position.y, 0.0f, 1080.0f);

        ImGui::Text("Escala:");
        ImGui::SliderFloat("Escala X", &scale.x, 0.1f, 5.0f);
        ImGui::SliderFloat("Escala Y", &scale.y, 0.1f, 5.0f);
        ImGui::End();
    }

    // Renderiza ImGui + SFML
    m_windowPtr->render();
    m_windowPtr->display();
}

void BaseApp::destroy() {
    // Recursos pueden liberarse aquí si es necesario
}