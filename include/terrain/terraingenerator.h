#ifndef _TERRAIN_GENERATOR_H_
#define _TERRAIN_GENERATOR_H_

#define TERRAIN_MIN_WIDTH 2
#define TERRAIN_MAX_WIDTH 100

#define TERRAIN_MIN_DEPTH 2
#define TERRAIN_MAX_DEPTH 100

#include "renderer/mesh.h"
#include "noise/perlinnoise.h"

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
};

class TerrainGenerator
{
public:
	TerrainGenerator(const TerrainConfig& config);
	~TerrainGenerator();

	void setMesh(Mesh& terrainMesh);
	void setShaderProgram(unsigned int shaderProgram);
	TerrainConfig& getConfig();
	void Apply() const;
private:
	TerrainConfig config;
	INoise* noise;
	Mesh* terrainMesh;
	unsigned int shaderProgram;
};

#endif