#ifndef _TEXTURE_H_
#define _TEXTURE_H_

class Texture
{
public:
	Texture(const char* path);
	Texture(unsigned char* data, int width, int height, int numChannels);
	~Texture();

	void Bind(unsigned int slot) const;

private:
	bool loadTexture(const char* path);

	unsigned int textureID;
	int width;
	int height;
	int numChannels;
};

#endif