#ifndef _SCENE_H_
#define _SCENE_H_

#define WORLD_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

#include "core/camera.h"

#include "renderer/terrainmesh.h"
#include "renderer/skybox.h"
#include "renderer/skyboxmesh.h"
#include "renderer/framedata.h"
#include "renderer/light.h"

#include "terrain/terraingenerator.h"
#include "terrain/terrainconfig.h"

#include "exporter/exporter.h"
#include "exporter/FBXexporter.h"
	
enum class UpdateSceneFlag : uint8_t
{
	None = 0,
	Mesh = 1 << 0,
	HeightMap = 1 << 1
};

class Scene
{
public:
	Scene();
	~Scene();

	void Update();
	void Generate();
	bool isGenerated() const;

	void FlagForUpdate(UpdateSceneFlag flag);
	bool ChangeSkybox(SkyboxOption option);
	void ExportTerrain(FileType type);

	TerrainConfig& getTerrainConfig();
	TerrainGenerator& getTerrainGenerator();
	TerrainMesh& getTerrainMesh();
	Transform& getTerrainTransform();
	Skybox& getSkybox();
	SkyboxMesh& getSkyboxMesh();
	Light& getLight();
	FrameData getFrameData(Camera& camera);

private:
	TerrainConfig config;
	TerrainGenerator terrainGenerator;
	TerrainMesh terrainMesh;
	Transform& terrainTransform;

	Skybox skybox;
	SkyboxMesh skyboxMesh;

	Light light;

	uint8_t flags;

	bool generated;
};

#endif