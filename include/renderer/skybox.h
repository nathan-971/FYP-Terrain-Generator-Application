#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include <vector>
#include <string>
#include <unordered_map>

#include "skyboxoption.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	bool Change(SkyboxOption& option);
	bool LoadTextures();
	bool isDisabled();
	unsigned int getActiveTextureId();

private:
	unsigned int loadSkyboxTexture(SkyboxOption& option);
	bool ensureCached(SkyboxOption option);
	bool isCached(SkyboxOption& option);

	unsigned int activeTexture;
	std::unordered_map<SkyboxOption, std::vector<std::string>> skyboxTexturePaths;
	std::unordered_map<SkyboxOption, unsigned int> skyboxCache;
};

#endif