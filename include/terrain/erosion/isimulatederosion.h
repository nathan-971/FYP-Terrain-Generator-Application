#ifndef _I_SIMULATED_H_
#define _I_SIMULATED_H_

#include "terrain/erosion/ierosion.h"
#include "terrain/heightmap.h"

class ISimulatedErosion : public IErosion
{
public:
	virtual ~ISimulatedErosion() = default;
	virtual void Reset() = 0;
	virtual void Step(HeightMap& heightmap, int dropsPerSecond) = 0;
	virtual bool Finished() const = 0;
};

#endif