#ifndef _TERRAIN_GENERATOR_H_
#define _TERRAIN_GENERATOR_H_

#include "renderer/terrainmesh.h"

#include "terrain/terrainconfig.h"
#include "terrain/iterraingenerator.h"
#include "terrain/height/iheightgenerator.h"
#include "terrain/erosion/ierosion.h"

#include <memory>

class TerrainGenerator : public ITerrainGenerator
{
public:
	TerrainGenerator(
		std::unique_ptr<IHeightGenerator> heightGenerator,
		std::unique_ptr<IErosion> erosion = nullptr
	);
	~TerrainGenerator();
	void Generate(TerrainMesh& mesh) override;
	void UpdateParameters(const TerrainConfig& config) override;

private:
	std::unique_ptr<IHeightGenerator> heightGenerator;
	std::unique_ptr<IErosion> erosion;
};

#endif