#ifndef _HYDRAULIC_EROSION_H_
#define _HYDRAULIC_EROSION_H_

#include "terrain/erosion/ierosion.h"
#include "terrain/erosion/hydraulicconfig.h"
#include "terrain/erosion/droplet.h"

#include <random>

class HydraulicErosion : public IErosion
{
public:
	HydraulicErosion(const HydraulicConfig& config, int seed);
	void Apply(HeightMap& heightmap) override;

private:
	HydraulicConfig config;
	std::mt19937 rng;
};

#endif