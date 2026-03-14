#ifndef _I_TERRAIN_SYSTEM_H_
#define _I_TERRAIN_SYSTEM_H_

#include "scene/mesh/transform.h"
#include "scene/terrain/terrainmesh.h"
#include "terrain/terrainconfig.h"

class ITerrainSystem
{
public:
	virtual ~ITerrainSystem() = default;

    virtual void Initalize(const TerrainConfig& config) = 0;
    virtual void Update(const TerrainConfig& config, float deltaTime) = 0;

    virtual void RebuildGenerator(const TerrainConfig& config) = 0;
    virtual void RebuildHeightMap(const TerrainConfig& config) = 0;
    virtual void RebuildMesh(int width, int depth, float resolution) = 0;

    virtual void StartErosion() = 0;
    virtual void StopErosion() = 0;
    virtual void ResetErosion() = 0;

    virtual TerrainMesh& getMesh() = 0;
    virtual Transform& getMeshTransform() = 0;
};

#endif