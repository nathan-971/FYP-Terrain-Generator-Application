#ifndef _I_TERRAIN_GENERATOR_H_
#define _I_TERRAIN_GENERATOR_H_

#include "renderer/terrainmesh.h"
#include "terrain/terrainconfig.h"

class ITerrainGenerator
{
public:
	virtual ~ITerrainGenerator() = default;
	virtual void Generate(TerrainMesh& mesh) = 0;
	virtual void UpdateParameters(const TerrainConfig& config) = 0;
};

#endif