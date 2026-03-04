#ifndef _UPDATE_SCENE_FLAG_H_
#define _UPDATE_SCENE_FLAG_H_

enum class UpdateSceneFlag : uint8_t
{
	None = 0,
	Mesh = 1 << 0,
	HeightMap = 1 << 1,
	RebuildTerrainGenerator = 1 << 2
};

#endif