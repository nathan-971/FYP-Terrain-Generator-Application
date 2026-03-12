#include "utils/terraingeneratorfactory.h"

#include "terrain/terraingenerator.h"

#include "terrain/height/iheightgenerator.h"
#include "terrain/height/ridgednoisegenerator.h"
#include "terrain/height/basenoisegenerator.h"

#include "terrain/noise/perlinnoise.h"

#include "terrain/warp/singlewarp.h"
#include "terrain/warp/doublewarp.h"

#include "terrain/erosion/hydraulicconfig.h"
#include "terrain/erosion/isimulatederosion.h"
#include "terrain/erosion/hydraulicerosion.h"

std::unique_ptr<ITerrainGenerator> TerrainGeneratorFactory::Create(const TerrainConfig& config)
{
	std::shared_ptr<INoise> noise = std::make_shared<PerlinNoise>();
	std::unique_ptr<IWarp> warp = createWarp(config);

	noise->ApplySeed(config.seed);
	std::unique_ptr<IHeightGenerator> height = createHeightGenerator(config, noise, std::move(warp));

	if (config.erosionEnabled)
	{
		std::unique_ptr<IErosionHandler> erosionHandler = createErosionHandler(config);
		return std::make_unique<TerrainGenerator>(
			std::move(height), 
			std::move(erosionHandler)
		);
	}

	return std::make_unique<TerrainGenerator>(std::move(height));
}

std::unique_ptr<IWarp> TerrainGeneratorFactory::createWarp(const TerrainConfig& config)
{
	switch(config.warpMode)
	{
		case WarpMode::Single:
		{
			return std::make_unique<SingleWarp>(config.warpFrequency, config.warpMultiplier);
		}
		case WarpMode::Double:
		{
			return std::make_unique<DoubleWarp>(config.warpFrequency, config.warpMultiplier);
		}
		case WarpMode::None:
		default:
		{
			return nullptr;
		}
	}
}

std::unique_ptr<IHeightGenerator> TerrainGeneratorFactory::createHeightGenerator(
	const TerrainConfig& config, 
	std::shared_ptr<INoise> noise, 
	std::unique_ptr<IWarp> warp)
{
	switch (config.noiseConfig)
	{
		case NoiseConfiguration::RidgedNoise:
		{
			return std::make_unique<RidgedNoiseGenerator>(config, noise, std::move(warp));
		}
		case NoiseConfiguration::BaseNoise:
		default:
		{
			return std::make_unique<BaseNoiseGenerator>(config, noise, std::move(warp));
		}
	}
}

std::unique_ptr<IErosionHandler> TerrainGeneratorFactory::createErosionHandler(const TerrainConfig& config)
{
	HydraulicConfig hydraulicConfig;
	std::unique_ptr<ISimulatedErosion> hydraulic = std::make_unique<HydraulicErosion>(
		hydraulicConfig, config.seed
	);
	return std::make_unique<ErosionHandler>(std::move(hydraulic));
}