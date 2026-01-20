#ifndef _TERRAIN_GENERATOR_H_
#define _TERRAIN_GENERATOR_H_

#define TERRAIN_MIN_WIDTH 2
#define TERRAIN_MAX_WIDTH 100

#define TERRAIN_MIN_DEPTH 2
#define TERRAIN_MAX_DEPTH 100

#include "renderer/mesh.h"
#include "noise/perlinnoise.h"
#include "terrain/terrainconfig.h"
#include <noise/noise.h>

class TerrainGenerator
{
public:
	TerrainGenerator(TerrainConfig& config);
	~TerrainGenerator();

	void setMesh(Mesh& terrainMesh);
	void setShaderProgram(unsigned int shaderProgram);
	void Apply() const;

	TerrainConfig& getConfig();
private:
	TerrainConfig& config;
	Noise* noise;
	Mesh* terrainMesh;
	unsigned int shaderProgram;
};

#endif