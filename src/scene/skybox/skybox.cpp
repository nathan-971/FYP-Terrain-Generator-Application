#include "scene/skybox/skybox.h"

#include "utils/resources.h"

#include <glad/glad.h>
#include "stb/stb_image.h"

Skybox::Skybox() 
	: activeTexture(0)
{
	skyboxTexturePaths[SkyboxOption::MORNING] = {
		Resources::AssetPath("textures\\skybox\\morningRight.jpg").string(),
		Resources::AssetPath("textures\\skybox\\morningLeft.jpg").string(),
		Resources::AssetPath("textures\\skybox\\morningTop.jpg").string(),
		Resources::AssetPath("textures\\skybox\\morningBottom.jpg").string(),
		Resources::AssetPath("textures\\skybox\\morningFront.jpg").string(),
		Resources::AssetPath("textures\\skybox\\morningBack.jpg").string()
	};

	skyboxTexturePaths[SkyboxOption::NOON] = {
		Resources::AssetPath("textures\\skybox\\noonRight.jpg").string(),
		Resources::AssetPath("textures\\skybox\\noonLeft.jpg").string(),
		Resources::AssetPath("textures\\skybox\\noonTop.jpg").string(),
		Resources::AssetPath("textures\\skybox\\noonBottom.jpg").string(),
		Resources::AssetPath("textures\\skybox\\noonFront.jpg").string(),
		Resources::AssetPath("textures\\skybox\\noonBack.jpg").string()
	};

	skyboxTexturePaths[SkyboxOption::NIGHT] = {
		Resources::AssetPath("textures\\skybox\\nightRight.jpg").string(),
		Resources::AssetPath("textures\\skybox\\nightLeft.jpg").string(),
		Resources::AssetPath("textures\\skybox\\nightTop.jpg").string(),
		Resources::AssetPath("textures\\skybox\\nightBottom.jpg").string(),
		Resources::AssetPath("textures\\skybox\\nightFront.jpg").string(),
		Resources::AssetPath("textures\\skybox\\nightBack.jpg").string()
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

unsigned int Skybox::getActiveTextureId() const
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

bool Skybox::isDisabled() const
{
	return activeTexture == skyboxCache.at(SkyboxOption::NONE);
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