#ifndef _HYDRAULIC_EROSION_H_
#define _HYDRAULIC_EROSION_H_

#include "terrain/erosion/ierosion.h"
#include "terrain/erosion/isimulatederosion.h"
#include "terrain/erosion/hydraulicconfig.h"
#include "terrain/erosion/droplet.h"
#include "terrain/heightmap.h"

#include <random>

class HydraulicErosion : public ISimulatedErosion
{
public:
	HydraulicErosion(const HydraulicConfig& config, int seed);
	~HydraulicErosion();
	void Apply(HeightMap& heightmap) override;

	void Step(HeightMap& heightmap, int dropsPerStep) override;
	void Reset() override;
	bool Finished() const override;

private:
	std::mt19937 rng;
	HydraulicConfig config;
	int seed = 0;
	int currentDrop = 0;
};

#endif