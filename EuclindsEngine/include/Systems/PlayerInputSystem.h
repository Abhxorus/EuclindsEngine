#pragma once
#include "Prerequisitos.h"
#include "ECS/A_Player.h"

struct
	PlayerInputConfig {
	EngineUtilities::TSharedPointer<A_Player> player;
	sf::RenderWindow* renderWindow = nullptr;
};

class
	PlayerInputSystem {
public:
	PlayerInputSystem(const PlayerInputConfig& cfg) : cfg(cfg) {}

	~PlayerInputSystem() = default;

	void
		update(float dt);

private:
	PlayerInputConfig cfg;

	void
		updateDirect(float dt);
	void
		updateTargetSeek();
};