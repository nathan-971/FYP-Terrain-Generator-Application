#ifndef _I_TERRAIN_GENERATOR_FACTORY_H_
#define _I_TERRAIN_GENERATOR_FACTORY_H_

#include "terrain/iterraingenerator.h"

#include <memory>

class ITerrainGeneratorFactory
{
public:
	virtual ~ITerrainGeneratorFactory() = default;
	virtual std::unique_ptr<ITerrainGenerator> Create(const TerrainConfig& config) = 0;
};

#endif