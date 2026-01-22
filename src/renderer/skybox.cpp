#include "renderer/skybox.h"

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

	loadAndCacheTextures(SkyboxOption::MORNING);
	loadAndCacheTextures(SkyboxOption::NOON);
	loadAndCacheTextures(SkyboxOption::NIGHT);

	activeTexture = skyboxCache[SkyboxOption::NOON];
	skyboxCache[SkyboxOption::NONE] = 0;
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

unsigned int Skybox::getActiveTextureId()
{
	return this->activeTexture;
}

bool Skybox::Change(SkyboxOption option)
{
	if (loadAndCacheTextures(option))
	{
		activeTexture = skyboxCache[option];
		return true;
	}
	return false;
}

bool Skybox::loadAndCacheTextures(SkyboxOption option)
{
	if (skyboxCache[option])
	{
		return true;
	}

	int numOfPaths = skyboxTexturePaths.at(option).size();
	if (numOfPaths != 6)
	{
		return false;
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
			stbi_image_free(texData);
			glDeleteTextures(1, &textureId);
			return false;
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

	skyboxCache[option] = textureId;
	return true;
}