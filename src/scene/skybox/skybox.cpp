#include "scene/skybox/skybox.h"
#include <glad/glad.h>
#include "stb/stb_image.h"

Skybox::Skybox() 
	: activeTexture(0)
{
	skyboxTexturePaths[SkyboxOption::MORNING] = {
		"assets/textures/skybox/morningRight.jpg",
		"assets/textures/skybox/morningLeft.jpg",
		"assets/textures/skybox/morningTop.jpg",
		"assets/textures/skybox/morningBottom.jpg",
		"assets/textures/skybox/morningFront.jpg",
		"assets/textures/skybox/morningBack.jpg"
	};

	skyboxTexturePaths[SkyboxOption::NOON] = {
		"assets/textures/skybox/noonRight.jpg",
		"assets/textures/skybox/noonLeft.jpg",
		"assets/textures/skybox/noonTop.jpg",
		"assets/textures/skybox/noonBottom.jpg",
		"assets/textures/skybox/noonFront.jpg",
		"assets/textures/skybox/noonBack.jpg"
	};

	skyboxTexturePaths[SkyboxOption::NIGHT] = {
		"assets/textures/skybox/nightRight.jpg",
		"assets/textures/skybox/nightLeft.jpg",
		"assets/textures/skybox/nightTop.jpg",
		"assets/textures/skybox/nightBottom.jpg",
		"assets/textures/skybox/nightFront.jpg",
		"assets/textures/skybox/nightBack.jpg"
	};

	stbi_set_flip_vertically_on_load(false);
}

Skybox::~Skybox()
{ 
	for (auto& [k, v] : skyboxCache)
	{
		if (v != 0)
		{
			glDeleteTextures(1, &v);
		}
	}
}

bool Skybox::LoadTextures()
{
	ensureCached(SkyboxOption::MORNING);
	ensureCached(SkyboxOption::NOON);
	ensureCached(SkyboxOption::NIGHT);
	skyboxCache[SkyboxOption::NONE] = 0;

	activeTexture = skyboxCache.at(SkyboxOption::NOON);
	return true;
}

unsigned int Skybox::getActiveTextureId()
{
	return this->activeTexture;
}

bool Skybox::Change(SkyboxOption& option)
{
	if (option == SkyboxOption::NONE)
	{
		activeTexture = skyboxCache[SkyboxOption::NONE];
		return true;
	}

	if (ensureCached(option))
	{
		activeTexture = skyboxCache[option];
		return true;
	}
	return false;
}

bool Skybox::isDisabled()
{
	return activeTexture == skyboxCache[SkyboxOption::NONE];
}

bool Skybox::ensureCached(SkyboxOption option)
{
	if (isCached(option))
	{
		return true;
	}

	unsigned int textureId = loadSkyboxTexture(option);
	if (textureId == 0)
	{
		return false;
	}

	skyboxCache[option] = textureId;
	return true;
}

bool Skybox::isCached(SkyboxOption& option)
{
	auto iterator = skyboxCache.find(option);
	return iterator != skyboxCache.end() && iterator->second != 0;
}

unsigned int Skybox::loadSkyboxTexture(SkyboxOption& option)
{
	int numOfPaths = skyboxTexturePaths.at(option).size();
	if (numOfPaths != 6)
	{
		return 0;
	}

	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	for (int i = 0; i < numOfPaths; i++)
	{
		int texWidth;
		int texHeight;
		int numChannels;
		unsigned char* texData;

		texData = stbi_load(skyboxTexturePaths[option][i].c_str(), &texWidth, &texHeight, &numChannels, 0);
		if (!texData)
		{
			glDeleteTextures(1, &textureId);
			return 0;
		}

		int channelFormat = numChannels == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			channelFormat,
			texWidth,
			texHeight,
			0,
			channelFormat,
			GL_UNSIGNED_BYTE,
			texData
		);
		stbi_image_free(texData);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureId;
}