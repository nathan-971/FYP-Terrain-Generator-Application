#ifndef _I_EROSION_H_
#define _I_EROSION_H_

#include "terrain/heightmap.h"

class IErosion
{
public:
	virtual ~IErosion() = default;
	virtual void Apply(HeightMap& heightmap) = 0;
};

#endif