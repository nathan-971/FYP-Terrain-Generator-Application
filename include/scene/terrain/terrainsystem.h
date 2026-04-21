#ifndef _TERRAIN_SYSTEM_H_
#define _TERRAIN_SYSTEM_H_

#include "scene/terrain/iterrainsystem.h"
#include "scene/terrain/terrainmesh.h"

#include "scene/texture/material.h"

#include "terrain/terrainconfig.h"
#include "terrain/iterraingenerator.h"

#include "utils/iterraingeneratorfactory.h"

#include <memory>

class TerrainSystem : public ITerrainSystem
{
public:
    TerrainSystem(std::unique_ptr<ITerrainGeneratorFactory> generatorFactory);
    ~TerrainSystem();

    void Initalize() override;
    void Update(float deltaTime) override;

    void RebuildGenerator() override;
    void RebuildHeightMap() override;
    void RebuildMesh() override;

    void StartErosion() override;
    void StopErosion() override;
    void ResetErosion() override;

    TerrainConfig& getConfig() override;
    glm::vec3 getTerrainPosition() const override;
    const TerrainMesh& getMesh() const override;
    const Transform& getMeshTransform() const override;

    const Material& getGrassMaterial() const override;
    const Material& getStoneMaterial() const override;

private:
    TerrainMesh mesh;
    Material matGrass;
    Material matStone;

    TerrainConfig config;
    std::unique_ptr<ITerrainGenerator> generator;
    std::unique_ptr<ITerrainGeneratorFactory> generatorFactory;
};

#endif