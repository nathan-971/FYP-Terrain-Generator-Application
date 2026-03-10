#ifndef _I_EROSION_HANDLER_H_
#define _I_EROSION_HANDLER_H_

#include "terrain/heightmap.h"

class IErosionHandler
{
public:
	virtual ~IErosionHandler() = default;
	virtual void Start(bool reset = true) = 0;
	virtual void Stop() = 0;
	virtual void Reset() = 0;
	virtual void Update() = 0;
	virtual void SetOriginalMap(const HeightMap& map) = 0;
	virtual HeightMap& GetHeightMap() = 0;
};

#endif