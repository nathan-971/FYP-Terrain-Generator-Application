#ifndef _UPDATE_SCENE_FLAG_H_
#define _UPDATE_SCENE_FLAG_H_

#include <cstdint>

enum class UpdateSceneFlag : uint8_t
{
	None = 0,
	TerrainMesh = 1 << 0,
	HeightMap = 1 << 1,
	RebuildTerrainGenerator = 1 << 2,
	ChangeSkybox = 1 << 3,
	StartErosion = 1 << 4,
	StopErosion = 1 << 5,
	ResetErosion = 1 << 6
};

#endif