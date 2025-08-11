#include "BaseApp.h"
#include "ResourceManager.h"
#include "ActorPicker.h"
#include "ActorSerializer.h"

/* circuit provider lives outside BaseApp */
#include "Circuits/Circuit1.h"

/**
 * @brief Destructor for the BaseApp class.
 *
 * This destructor ensures that any resources tied to the BaseApp instance
 * are properly released upon its destruction.
 */
BaseApp::~BaseApp() {}

/**
 * @brief The main application loop.
 *
 * This function handles the application's core lifecycle: initialization,
 * the main event loop, and final resource destruction.
 *
 * @return An integer representing the application's exit code.
 */
int BaseApp::run() {
    if (!init()) {
        ERROR("BaseApp", "run", "Initialization failed.");
        return -1; // Added a proper error return value.
    }

    while (m_windowPtr->isOpen()) {
        m_windowPtr->handleEvents(m_engineGUI);
        ImGui::SFML::Update(*m_windowPtr->m_windowPtr, m_windowPtr->deltaTime);
        update();
        render();
    }

    destroy();
    return 0;
}

/**
 * @brief Initializes the application and its main components.
 *
 * This function sets up the window, the GUI, and creates the main
 * game actors (player, track, and NPC).
 *
 * @return true if initialization is successful, false otherwise.
 */
bool BaseApp::init() {
    ResourceManager& resourceMan = ResourceManager::getInstance();

    m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "EuclindsEngine");
    if (m_windowPtr.isNull()) {
        ERROR("BaseApp", "init", "Failed to create window pointer.");
        return false;
    }

    m_engineGUI.init(m_windowPtr);

    /* --- Create main actors (player, track, NPC) --- */

    // Initialize track actor
    m_ATrack = EngineUtilities::MakeShared<Actor>("Track Actor");
    if (m_ATrack) {
        m_ATrack->getComponent<CShape>()->createShape(RECTANGLE);
        m_ATrack->getComponent<CShape>()->setFillColor(sf::Color::White);
        m_ATrack->getComponent<Transform>()->setPosition(sf::Vector2f(450.f, 0.f));
        m_ATrack->getComponent<Transform>()->setScale(sf::Vector2f(10.f, 19.5f));
        if (!resourceMan.loadTexture("sprites/Track", "png")) {
            MESSAGE("BaseApp", "Init", "Can't load texture.");
        }
        m_ATrack->setTexture(resourceMan.getTexture("sprites/Track"));
    }

    // Initialize player actor
    m_player = EngineUtilities::MakeShared<A_Player>("Player");
    if (m_player) {
        m_player->getComponent<CShape>()->createShape(CIRCLE);
        m_player->getComponent<CShape>()->setFillColor(sf::Color::Cyan);
        m_player->getComponent<Transform>()->setScale(sf::Vector2f(2.f, 2.f));
        m_player->setPosition(EngineMathLib::CVector2(880.f, 880.f));

        /* Default: direct control */
        m_player->setControlMode(PlayerControlMode::Direct);
        m_player->setAcceleration(700.f);
        m_player->setFriction(6.f);
        m_player->setMaxSpeed(m_sharedMaxSpeed);

        if (!resourceMan.loadTexture("sprites/mushroom", "png")) {
            MESSAGE("BaseApp", "Init", "Can't load texture.");
        }
        m_player->setTexture(resourceMan.getTexture("sprites/mushroom"));
    }

    // Initialize NPC racer
    m_racerNPC = EngineUtilities::MakeShared<A_Racer>("NPC_1");
    if (m_racerNPC) {
        m_racerNPC->getComponent<CShape>()->createShape(CIRCLE);
        m_racerNPC->getComponent<CShape>()->setFillColor(sf::Color::Green);
        m_racerNPC->getComponent<Transform>()->setScale(sf::Vector2f(2.f, 2.f));
        if (!resourceMan.loadTexture("sprites/mushroom", "png")) {
            MESSAGE("BaseApp", "Init", "Can't load texture.");
        }
        m_racerNPC->setTexture(resourceMan.getTexture("sprites/mushroom"));

        m_racerNPC->setSpeed(m_sharedMaxSpeed);
        m_racerNPC->setArriveRadius(40.f);
        m_racerNPC->setMode(SteeringMode::Arrive);
        m_racerNPC->enableSteering(false);
        m_racerNPC->setPosition(EngineMathLib::CVector2(855.f, 855.f));
    }
    m_npcs.clear();
    if (!m_racerNPC.isNull()) {
        m_npcs.push_back(m_racerNPC);
    }

    /* --- Waypoints --- */
    m_waypoints = getCircuitWaypoints();

    /* --- Systems initialization --- */
    {
        PlayerInputConfig pic;
        pic.player = m_player;
        pic.renderWindow = m_windowPtr->m_windowPtr.get();
        m_playerInputSystem = EngineUtilities::MakeUnique<PlayerInputSystem>(pic);
    }

    {
        WaypointFollowConfig wfc;
        wfc.racers = m_npcs;
        wfc.waypoints = &m_waypoints;
        wfc.arriveRadiusForAdvance = 22.f;
        wfc.reactionDelay = 0.14f;
        wfc.waypointNoiseRadius = 6.f;
        wfc.lookAhead = 1;
        wfc.mistakeProb = 0.03f;
        wfc.mistakeCooldown = 2.0f;
        wfc.corridorWidth = 80.f;
        wfc.cornerSlowdownEnabled = true;
        wfc.cornerMinAngleDeg = 25.f;
        wfc.cornerMaxAngleDeg = 95.f;
        wfc.cornerMinFactor = 0.82f;
        m_waypointFollowSystem = EngineUtilities::MakeUnique<WaypointFollowSystem>(wfc);
        m_waypointFollowSystem->primeFromPositions();
    }

    {
        SteeringConfig sc;
        sc.racers = m_npcs;
        sc.seekRange = 10.f;
        m_steeringSystem = EngineUtilities::MakeUnique<SteeringSystem>(sc);
    }

    {
        RaceConfig rc;
        rc.actors.clear();
        if (!m_player.isNull()) {
            rc.actors.push_back(m_player);
        }
        for (auto& npc : m_npcs) {
            rc.actors.push_back(npc);
        }
        rc.waypoints = &m_waypoints;
        rc.checkpointRadius = 12.f;
        m_raceSystem = EngineUtilities::MakeUnique<RaceSystem>(rc);
    }

    /* --- Arm and start countdown --- */
    m_countdown.start();
    m_raceArmed = true;
    m_raceLive = false;
    m_raceFinished = false;
    m_finalPlace = -1;
    m_npcFinished = false;

    return true;
}

/**
 * @brief Updates the state of the game logic.
 *
 * This function is called once per frame. It manages the race countdown,
 * enforces speed limits, and updates the various game systems.
 */
void BaseApp::update() {
    if (m_windowPtr.isNull()) return;

    m_windowPtr->update();
    const float dt = m_windowPtr->deltaTime.asSeconds();

    /* --- Race countdown gate --- */
    std::string cdText;
    if (m_raceArmed && !m_raceLive) {
        cdText = m_countdown.tick(dt);
        if (m_countdown.isFinished()) {
            m_raceLive = true;
            m_raceArmed = false;

            /* enable NPC steering now that waypoints will start updating */
            if (!m_racerNPC.isNull()) {
                m_racerNPC->enableSteering(true);
            }
            /* activate timing on GO */
            if (!m_raceSystem.isNull()) {
                m_raceSystem->setTimingActive(true);
            }
        }
    }

    /* Enforce player speed and NPC handicap every frame */
    if (!m_player.isNull()) {
        float playerMax = m_player->getMaxSpeed();
        if (playerMax <= 0.f) playerMax = m_sharedMaxSpeed;
        m_player->setMaxSpeed(playerMax);

        if (!m_racerNPC.isNull()) {
            float npcMax = playerMax * m_npcSpeedFactor; /* a tad slower than player */
            m_racerNPC->setSpeed(npcMax);                /* WaypointFollow may lower more in corners */
        }
    }

    /* --- Systems update order --- */
    if (m_raceLive && !m_raceFinished) {
        if (!m_playerInputSystem.isNull()) { m_playerInputSystem->update(dt); }
        if (!m_waypointFollowSystem.isNull()) { m_waypointFollowSystem->update(dt); }
        if (!m_steeringSystem.isNull()) { m_steeringSystem->update(dt); }
    }

    if (!m_raceSystem.isNull()) { m_raceSystem->update(dt); }

    /* --- Detect finish (3 laps) --- */
    if (!m_raceFinished && !m_raceSystem.isNull()) {
        int playerLap = m_raceSystem->getLapData(0).lap;
        int npcLap = (m_raceSystem->getLapData(1).lap);

        if (!m_npcFinished && npcLap >= m_lapsToWin) {
            m_npcFinished = true;
            if (!m_racerNPC.isNull()) {
                m_racerNPC->enableSteering(false);
                m_racerNPC->setSpeed(0.f);
            }
        }

        if (playerLap >= m_lapsToWin) {
            m_raceFinished = true;
            m_raceLive = false; /* freeze */
            std::vector<int> order = m_raceSystem->getStandings();
            m_finalPlace = -1;
            for (size_t k = 0; k < order.size(); ++k) {
                if (order[k] == 0) {  /* index 0 = Player */
                    m_finalPlace = static_cast<int>(k + 1);
                    break;
                }
            }
        }
    }

    /* --- Update all actors EVERY frame --- */
    if (!m_ATrack.isNull()) m_ATrack->update(dt);
    if (!m_player.isNull()) m_player->update(dt);
    if (!m_racerNPC.isNull()) m_racerNPC->update(dt);

    /* --- Prepare actors vector for GUI panels --- */
    actorsVector.clear();
    if (!m_ATrack.isNull()) actorsVector.push_back(m_ATrack);
    if (!m_player.isNull()) actorsVector.push_back(m_player);
    if (!m_racerNPC.isNull()) actorsVector.push_back(m_racerNPC);

    /* --- Panels: Menu, Hierarchy, Inspector, Console, FileManager --- */
    m_engineGUI.menuBar();
    m_engineGUI.hierarchy(actorsVector);
    m_engineGUI.inspector(actorsVector);
    m_engineGUI.console();
    m_engineGUI.fileManagerPanel(actorsVector);

    /* --- Picking (actor selection) --- */
    if (!ImGui::GetIO().WantCaptureMouse && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        int pickedIdx = ActorPicker::pickActorUnderMouse(m_windowPtr->m_windowPtr.get(), actorsVector);
        if (pickedIdx != -1) {
            m_engineGUI.selectedActorIndex = pickedIdx;
        }
    }

    /* --- Countdown overlay (transparent, no input capture; does NOT block ImGui panels) --- */
    if (!m_raceLive && !m_raceFinished && !cdText.empty()) {
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::SetNextWindowPos(
            ImVec2((float)m_windowPtr->m_windowPtr->getSize().x * 0.5f,
                (float)m_windowPtr->m_windowPtr->getSize().y * 0.35f),
            ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        ImGui::Begin("##countdown_overlay", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("%s", cdText.c_str());
        ImGui::End();
    }

    /* --- HUD: Lap / Position (top-left) using RaceSystem::getStandings() + Timers --- */
    if (!m_raceSystem.isNull() && !m_raceFinished) {
        std::vector<int> order = m_raceSystem->getStandings();

        int place = -1;
        for (size_t k = 0; k < order.size(); ++k) {
            if (order[k] == 0) {  /* index 0 = Player */
                place = static_cast<int>(k + 1);
                break;
            }
        }

        int lapHUD_real = m_raceSystem->getLapData(0).lap;
        int lapHUD = lapHUD_real + 1; /* VISUAL: starts at 1 */

        /* format helper mm:ss.cc */
        auto fmt = [](float sec) -> std::string {
            if (sec < 0.f) return std::string("--:--.--");
            int total_ms = static_cast<int>(sec * 1000.f + 0.5f);
            int minutes = total_ms / 60000;
            int seconds = (total_ms / 1000) % 60;
            int hundred = (total_ms % 1000) / 10;
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%02d:%02d.%02d", minutes, seconds, hundred);
            return std::string(buf);
            };

        float bestLap = m_raceSystem->getPlayerBestLapTime();  /* -1 if not yet set */
        float currLap = m_raceSystem->getPlayerCurrentLapTime();

        ImGui::SetNextWindowBgAlpha(0.3f);
        ImGui::SetNextWindowPos(ImVec2(40.f, 40.f), ImGuiCond_Always);
        ImGui::Begin("##race_hud_lappos", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::SetWindowFontScale(2.0f);
        ImGui::Text("Lap: %d / %d", lapHUD, m_lapsToWin);
        if (place > 0) ImGui::Text("Position: %d", place);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();
        ImGui::Text("Best:    %s", fmt(bestLap).c_str());
        ImGui::Text("Current: %s", fmt(currLap).c_str());

        ImGui::End();
    }

    /* --- Finish overlay (result + retry) --- */
    if (m_raceFinished) {
        ImGui::SetNextWindowBgAlpha(0.2f);
        ImGui::SetNextWindowPos(
            ImVec2((float)m_windowPtr->m_windowPtr->getSize().x * 0.5f,
                (float)m_windowPtr->m_windowPtr->getSize().y * 0.5f),
            ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        ImGui::Begin("##finish_overlay", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::SetWindowFontScale(3.0f);
        if (m_finalPlace == 1) {
            ImGui::Text("¡Ganaste! 1ro");
        }
        else if (m_finalPlace > 0) {
            ImGui::Text("Terminaste %do", m_finalPlace);
        }
        else {
            ImGui::Text("Carrera terminada");
        }
        ImGui::SetWindowFontScale(1.2f);

        ImGui::Spacing();
        if (ImGui::Button("Reintentar", ImVec2(240.f, 60.f))) {
            resetRace();
        }

        ImGui::End();
    }
}

/**
 * @brief Renders the game scene and GUI elements.
 *
 * This function clears the window, draws all actors, renders
 * the GUI, and displays the final frame.
 */
void BaseApp::render() {
    if (m_windowPtr.isNull()) return;

    m_windowPtr->clear();

    /* --- Draw scene actors --- */
    if (m_ATrack) m_ATrack->getComponent<CShape>()->render(m_windowPtr);
    if (m_player) m_player->getComponent<CShape>()->render(m_windowPtr);
    if (m_racerNPC) m_racerNPC->getComponent<CShape>()->render(m_windowPtr);

    /* --- Draw outline for selected actor --- */
    m_engineGUI.drawSelectedOutline(m_windowPtr->m_windowPtr.get(), actorsVector);

    /* --- Render ImGui panels --- */
    m_windowPtr->render();
    m_engineGUI.render(m_windowPtr);
    m_windowPtr->display();
}

/**
 * @brief Cleans up resources before the application closes.
 */
void BaseApp::destroy() {
    m_engineGUI.destroy();
}

/**
 * @brief Resets the state of the race.
 *
 * This helper function restores actors to their starting positions
 * and resets all race-related systems and flags.
 */
void BaseApp::resetRace() {
    /* reset actors to initial positions */
    if (!m_player.isNull()) {
        m_player->setPosition(EngineMathLib::CVector2(880.f, 880.f));
    }
    if (!m_racerNPC.isNull()) {
        m_racerNPC->setPosition(EngineMathLib::CVector2(855.f, 855.f));
        m_racerNPC->enableSteering(false);
    }

    /* rebuild systems that depend on state */
    {
        WaypointFollowConfig wfc;
        wfc.racers = m_npcs;
        wfc.waypoints = &m_waypoints;
        wfc.arriveRadiusForAdvance = 22.f;
        wfc.reactionDelay = 0.14f;
        wfc.waypointNoiseRadius = 6.f;
        wfc.lookAhead = 1;
        wfc.mistakeProb = 0.03f;
        wfc.mistakeCooldown = 2.0f;
        wfc.corridorWidth = 80.f;
        wfc.cornerSlowdownEnabled = true;
        wfc.cornerMinAngleDeg = 25.f;
        wfc.cornerMaxAngleDeg = 95.f;
        wfc.cornerMinFactor = 0.82f;

        m_waypointFollowSystem = EngineUtilities::MakeUnique<WaypointFollowSystem>(wfc);
        m_waypointFollowSystem->primeFromPositions();
    }

    {
        RaceConfig rc;
        rc.actors.clear();
        if (!m_player.isNull()) rc.actors.push_back(m_player);
        for (auto& npc : m_npcs) rc.actors.push_back(npc);
        rc.waypoints = &m_waypoints;
        rc.checkpointRadius = 12.f;
        m_raceSystem = EngineUtilities::MakeUnique<RaceSystem>(rc);
    }

    /* reset countdown and flags */
    m_countdown = RaceCountdown{ 3.f };
    m_countdown.start();
    m_raceArmed = true;
    m_raceLive = false;
    m_raceFinished = false;
    m_finalPlace = -1;
    m_npcFinished = false;

    /* stop timing until the next GO */
    if (!m_raceSystem.isNull()) {
        m_raceSystem->setTimingActive(false);
    }
}
