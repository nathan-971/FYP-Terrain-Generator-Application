#include "terrain/erosion/erosionhandler.h"

ErosionHandler::ErosionHandler(std::unique_ptr<ISimulatedErosion> simulatedErosion)
	: simulatedErosion(std::move(simulatedErosion)), running(false) { }

ErosionHandler::~ErosionHandler() { }

void ErosionHandler::Start(bool reset)
{
	if (reset)
	{
		this->Reset();
	}
	running = true;
}

void ErosionHandler::Stop()
{
	running = false;
}

void ErosionHandler::Reset()
{
	erosionMap = originalMap;

	if (simulatedErosion)
	{
		simulatedErosion->Reset();
	}
}

void ErosionHandler::Update()
{
	if (!running || !simulatedErosion)
	{
		return;
	}

	simulatedErosion->Step(erosionMap, 20);
	if (simulatedErosion->Finished())
	{
		running = false;
	}
}

void ErosionHandler::SetOriginalMap(const HeightMap& map)
{
	originalMap = map;
	erosionMap = map;

	if (simulatedErosion)
	{
		simulatedErosion->Reset();
	}
}

HeightMap& ErosionHandler::GetHeightMap()
{
	return erosionMap;
}