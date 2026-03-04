#ifndef _I_HEIGHT_GENERATOR_H_
#define _I_HEIGHT_GENERATOR_H_

#include "terrain/heightmap.h"

class IHeightGenerator 
{
public:
    virtual ~IHeightGenerator() = default;
    virtual void Generate(HeightMap& map) = 0;
    virtual void SetConfig(const TerrainConfig& config) = 0;
};

#endif