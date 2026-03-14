#ifndef _TERRAIN_SYSTEM_H_
#define _TERRAIN_SYSTEM_H_

#include "scene/terrain/iterrainsystem.h"
#include "scene/terrain/terrainmesh.h"

#include "terrain/terrainconfig.h"
#include "terrain/iterraingenerator.h"

#include "utils/iterraingeneratorfactory.h"

#include <memory>

class TerrainSystem : public ITerrainSystem
{
public:
    TerrainSystem(std::unique_ptr<ITerrainGeneratorFactory> generatorFactory);
    ~TerrainSystem();

    void Initalize(const TerrainConfig& config) override;
    void Update(const TerrainConfig& config, float deltaTime) override;

    void RebuildGenerator(const TerrainConfig& config) override;
    void RebuildHeightMap(const TerrainConfig& config) override;
    void RebuildMesh(int width, int depth, float resolution) override;

    void StartErosion() override;
    void StopErosion() override;
    void ResetErosion() override;

    TerrainMesh& getMesh() override;
    Transform& getMeshTransform() override;

private:
    TerrainMesh mesh;
    std::unique_ptr<ITerrainGenerator> generator;
    std::unique_ptr<ITerrainGeneratorFactory> generatorFactory;
};

#endif