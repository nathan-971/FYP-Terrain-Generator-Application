#ifndef _EROSION_HANDLER_H_
#define _EROSION_HANDLER_H_

#include "terrain/heightmap.h"
#include "terrain/erosion/isimulatederosion.h"
#include "terrain/erosion/ierosionhandler.h"

#include <memory>

class ErosionHandler : public IErosionHandler
{
public:
	ErosionHandler(std::unique_ptr<ISimulatedErosion> simulatedErosion = nullptr);
	~ErosionHandler();

	void Start() override;
	void Stop() override;
	void Reset() override;
	void Update() override;

	HeightMap& GetHeightMap();
	void SetOriginalMap(const HeightMap& map);

private:
	std::unique_ptr<ISimulatedErosion> simulatedErosion;
	HeightMap originalMap;
	HeightMap erosionMap;
	bool running;
};

#endif