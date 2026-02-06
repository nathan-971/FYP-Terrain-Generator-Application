#ifndef _TERRAIN_GENERATOR_H_
#define _TERRAIN_GENERATOR_H_

#define TERRAIN_MIN_WIDTH 25
#define TERRAIN_MAX_WIDTH 100

#define TERRAIN_MIN_DEPTH 25
#define TERRAIN_MAX_DEPTH 100

#include "renderer/vertex.h"
#include "renderer/terrainmesh.h"
#include "noise/perlinnoise.h"
#include "terrain/terrainconfig.h"
#include <noise/noise.h>

enum class NoiseConfiguration
{
	BaseNoise,
	RidgedNoise
};

enum class WarpMode
{
	None,
	Single,
	Double
};

class TerrainGenerator
{
public:
	TerrainGenerator(TerrainConfig& config);
	~TerrainGenerator();

	void setMesh(TerrainMesh& terrainMesh);
	void setSeed(int& seed);
	void setShaderProgram(unsigned int shaderProgram);
	void setNoiseConfiguration(NoiseConfiguration& noiseConfig);
	void setWarpMode(WarpMode& warpMode);
	void toggleErosion();
	void Apply();

	TerrainConfig& getConfig();
private:
	void applyBaseNoise();
	void applyRidgedNoise();
	void applyErosion();
	void warpSingle(Vertex& v, float& wx, float& wz);
	void warpDouble(Vertex& v, float& wx, float& wz);
	void warp(Vertex& v, float& wx, float& wz, float frequency, float multiplier);

	NoiseConfiguration noiseConfig;
	WarpMode warpMode;

	TerrainConfig& config;
	Noise* noise;
	TerrainMesh* terrainMesh;
	unsigned int shaderProgram;
	int seed;
	bool erosionToggled;
};

#endif