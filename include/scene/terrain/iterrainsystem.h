#ifndef _I_TERRAIN_SYSTEM_H_
#define _I_TERRAIN_SYSTEM_H_

#include "scene/mesh/transform.h"
#include "scene/terrain/terrainmesh.h"
#include "scene/texture/material.h"

#include "terrain/terrainconfig.h"

#include <glm/glm.hpp>

class ITerrainSystem
{
public:
	virtual ~ITerrainSystem() = default;

    virtual void Initalize() = 0;
    virtual void Update(float deltaTime) = 0;

    virtual void RebuildGenerator() = 0;
    virtual void RebuildHeightMap() = 0;
    virtual void RebuildMesh() = 0;

    virtual void StartErosion() = 0;
    virtual void StopErosion() = 0;
    virtual void ResetErosion() = 0;
    
    virtual TerrainConfig& getConfig() = 0;
    virtual glm::vec3 getTerrainPosition() const = 0;
    virtual const TerrainMesh& getMesh() const = 0;
    virtual const Transform& getMeshTransform() const = 0;

    virtual const Material& getGrassMaterial() const = 0;
    virtual const Material& getStoneMaterial() const = 0;
};

#endif