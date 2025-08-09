#pragma once
#include "Prerequisitos.h"
#include "ECS/Texture.h"

class
	ResourceManager
{
public:
	ResourceManager() {};
	~ResourceManager() {};

private:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

public:
	static ResourceManager&
		getInstance() {
		static ResourceManager instance;
		return instance;
	}

	bool
		loadTexture(const std::string& filename, const std::string& extension);

	EngineUtilities::TSharedPointer<Texture>
		getTexture(const std::string& fileName);

private:
	std::unordered_map < std::string, EngineUtilities::TSharedPointer<Texture>>
		m_textures;

};