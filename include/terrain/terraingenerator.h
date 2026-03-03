#ifndef _TERRAIN_GENERATOR_H_
#define _TERRAIN_GENERATOR_H_

#define TERRAIN_MIN_WIDTH 25
#define TERRAIN_MAX_WIDTH 100

#define TERRAIN_MIN_DEPTH 25
#define TERRAIN_MAX_DEPTH 100

#include "renderer/vertex.h"
#include "renderer/terrainmesh.h"
#include "terrain/terrainconfig.h"
#include "noise/noise.h"
#include "noise/noiseconfiguration.h"
#include "noise/warpmode.h"

struct Droplet
{
	float posX = 0.0f;
	float posZ = 0.0f;
	
	float dirX = 0.0f;
	float dirZ = 0.0f;

	float speed = 1.0f;
	float water = 1.0f;
	float sediment = 0.0f;
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
	void setErosionEnabled(bool& erosionEnabled);
	void Apply();

	TerrainConfig& getConfig();
private:
	void applyBaseNoise();
	void applyRidgedNoise();
	void applyHydraulicErosion();
	void warpSingle(Vertex& v, float& wx, float& wz);
	void warpDouble(Vertex& v, float& wx, float& wz);
	void warp(Vertex& v, float& wx, float& wz, float frequency, float multiplier);

	float sampleHeight(const std::vector<float>& heightMap, float x, float z, int width, int depth);
	float& getHeight(std::vector<float>& heightMap, int x, int z, int width);

	NoiseConfiguration noiseConfig;
	WarpMode warpMode;

	TerrainConfig& config;
	Noise* noise;
	TerrainMesh* terrainMesh;
	unsigned int shaderProgram;
	int seed;
	bool erosionEnabled;
};

#endif