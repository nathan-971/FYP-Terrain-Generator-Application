#ifndef _I_WARP_H_
#define _I_WARP_H_

#include "terrain/noise/inoise.h"

class IWarp
{
public:
	virtual ~IWarp() = default;
	virtual void Apply(float& x, float& z, INoise& noise) const = 0;
	virtual void UpdateParameters(float freq, float mult) = 0;
};

#endif