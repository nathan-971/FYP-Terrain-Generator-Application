#ifndef _TERRAIN_GENERATOR_FACTORY_H_
#define _TERRAIN_GENERATOR_FACTORY_H_

#include "terrain/iterraingenerator.h"
#include "terrain/noise/inoise.h"
#include "terrain/height/iheightgenerator.h"
#include "terrain/warp/iwarp.h"
#include "terrain/erosion/erosionhandler.h"

#include "utils/iterraingeneratorfactory.h"

class TerrainGeneratorFactory : public ITerrainGeneratorFactory
{
public:
	std::unique_ptr<ITerrainGenerator> Create(const TerrainConfig& config) override;

private:
	std::unique_ptr<IWarp> createWarp(const TerrainConfig& config);
	std::unique_ptr<IErosionHandler> createErosionHandler(const TerrainConfig& config);
	std::unique_ptr<IHeightGenerator> createHeightGenerator(
		const TerrainConfig& config,
		std::shared_ptr<INoise> noise,
		std::unique_ptr<IWarp> warp
	);
};

#endif