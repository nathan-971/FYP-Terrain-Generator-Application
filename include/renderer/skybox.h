#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

#include <glad/glad.h>
#include "stb/stb_image.h"

enum class SkyboxOption
{
	NONE,
	MORNING,
	NOON,
	NIGHT
};

class Skybox
{
public:
	Skybox();
	~Skybox();

	bool Change(SkyboxOption option);
	bool LoadTextures();
	unsigned int getActiveTextureId();

private:
	bool loadAndCacheTextures(SkyboxOption option);

	unsigned int activeTexture;
	std::unordered_map<SkyboxOption, std::vector<std::string>> skyboxTexturePaths;
	std::unordered_map<SkyboxOption, unsigned int> skyboxCache;
};

#endif