#include "scene/texture/texture.h"

#include <glad/glad.h>
#include <stdexcept>
#include "stb/stb_image.h"

Texture::Texture(const char* path) :
	textureID(0), width(0), height(0), numChannels(0)
{
	if (!loadTexture(path))
	{
		throw std::runtime_error(std::string("Unable to Load Texture: ") + path);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

Texture::Texture(unsigned char* data, int width, int height, int numChannels)
	: textureID(0), width(width), height(height), numChannels(numChannels)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int channelFormat = 0;
	switch (this->numChannels)
	{
		case 1:
			channelFormat = GL_R;
			break;
		case 3:
			channelFormat = GL_RGB;
			break;
		case 4:
			channelFormat = GL_RGBA;
			break;
		default:
			throw std::runtime_error(std::string("Unable to Load Texture: "));
	}

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		channelFormat,
		this->width,
		this->height,
		0,
		channelFormat,
		GL_UNSIGNED_BYTE, 
		data
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

bool Texture::loadTexture(const char* path)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(path, &width, &height, &numChannels, 0);
	if (!data)
	{
		glDeleteTextures(1, &textureID);
		return false;
	}

	int channelFormat = 0;
	switch (numChannels)
	{
		case 1:
			channelFormat = GL_R;
			break;
		case 3:
			channelFormat = GL_RGB;
			break;
		case 4:
			channelFormat = GL_RGBA;
			break;
		default:
			return false;
	}

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		channelFormat,
		width,
		height,
		0,
		channelFormat,
		GL_UNSIGNED_BYTE,
		data
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return true;
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}