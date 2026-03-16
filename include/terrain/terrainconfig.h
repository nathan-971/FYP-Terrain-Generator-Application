#ifndef _TERRAIN_CONFIG_H_
#define _TERRAIN_CONFIG_H_

#include "scene/skybox/skyboxoption.h"

#include "terrain/warp/warpmode.h"
#include "terrain/noise/noiseconfiguration.h"

struct TerrainConfig
{
	//Mesh Config
	unsigned int width;
	unsigned int depth;
	float resolution;
	float rotationSpeed;

	//TG Config
	int octaves;
	int seed;
	float amplitude;
	float frequency;
	float persistence;
	float lacunarity;
	float scale;
	float warpMultiplier;
	float warpFrequency;

	//Erosion
	bool erosionEnabled;

	//Enumerated Configurations
	WarpMode warpMode;
	NoiseConfiguration noiseConfig;
};

#endif