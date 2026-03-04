#ifndef _BASE_RIDGED_GENERATOR_H_
#define _BASE_RIDGED_GENERATOR_H_

#include "terrain/terrainconfig.h"
#include "terrain/noise/inoise.h"
#include "terrain/warp/iwarp.h"
#include "terrain/height/iheightgenerator.h"

#include <memory>

class RidgedNoiseGenerator : public IHeightGenerator
{
public:
	RidgedNoiseGenerator(const TerrainConfig& config, std::shared_ptr<INoise> noise, std::shared_ptr<IWarp> warp);
	void Generate(HeightMap& map) override;
	void SetConfig(const TerrainConfig& config) override;

private:
	TerrainConfig config;
	std::shared_ptr<INoise> noise;
	std::shared_ptr<IWarp> warp;
};

#endif