// ActorSerializer.h
#pragma once
#include "ECS/Actor.h"
#include "Prerequisitos.h"
namespace ActorSerializer {
	bool
		saveActorsToFile(const std::string& filename, const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);
	bool
		loadActorsFromFile(const std::string& filename, std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);
}