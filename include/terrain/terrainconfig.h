#ifndef _TERRAIN_CONFIG_H_
#define _TERRAIN_CONFIG_H_

struct TerrainConfig
{
	unsigned int width;
	unsigned int depth;
	float resolution;
	int octaves;
	float amplitude;
	float frequency;
	float persistence;
	float lacunarity;
	float scale;

	float warpMultiplier;
	float warpFrequency;

	float rotationSpeed;

	bool enableErosion;
};

#endif