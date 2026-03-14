#ifndef _TERRAIN_GENERATOR_H_
#define _TERRAIN_GENERATOR_H_

#include "scene/terrain/terrainmesh.h"

#include "terrain/terrainconfig.h"
#include "terrain/iterraingenerator.h"
#include "terrain/height/iheightgenerator.h"
#include "terrain/erosion/ierosion.h"
#include "terrain/erosion/ierosionhandler.h"

#include <memory>

class TerrainGenerator : public ITerrainGenerator
{
public:
	TerrainGenerator(
		std::unique_ptr<IHeightGenerator> heightGenerator,
		std::unique_ptr<IErosionHandler> erosionHandler = nullptr
	);
	~TerrainGenerator();
	void Generate(TerrainMesh& mesh) override;
	void Update(TerrainMesh& mesh) override;
	void UpdateParameters(const TerrainConfig& config) override;

	void StartErosion() override;
	void StopErosion() override;
	void ResetErosion() override;

private:
	std::unique_ptr<IHeightGenerator> heightGenerator;
	std::unique_ptr<IErosionHandler> erosionHandler;
};

#endif