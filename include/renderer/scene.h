#ifndef _SCENE_H_
#define _SCENE_H_

#define WORLD_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "core/icamera.h"

#include "renderer/terrainmesh.h"
#include "renderer/skybox.h"
#include "renderer/skyboxmesh.h"
#include "renderer/framedata.h"
#include "renderer/light.h"
#include "renderer/updatesceneflag.h"

#include "terrain/terraingenerator.h"
#include "terrain/terrainconfig.h"

#include "exporter/exporter.h"
#include "exporter/FBXexporter.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Update(float deltaTime);
	void Generate();
	bool isGenerated() const;

	void FlagForUpdate(UpdateSceneFlag flag);
	bool ChangeSkybox(SkyboxOption option);
	void ExportTerrain(FileType type);

	FrameData getFrameData(const ICamera& camera);
	TerrainConfig& getTerrainConfig();
	TerrainGenerator& getTerrainGenerator();
	TerrainMesh& getTerrainMesh();
	Transform& getTerrainTransform();
	Skybox& getSkybox();
	SkyboxMesh& getSkyboxMesh();
	Light& getLight();

	void positionAndOrientateCamera(ICamera& camera);

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