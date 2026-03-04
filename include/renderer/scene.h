#ifndef _SCENE_H_
#define _SCENE_H_

#define WORLD_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

#include "core/icamera.h"

#include "renderer/terrainmesh.h"
#include "renderer/skybox.h"
#include "renderer/skyboxmesh.h"
#include "renderer/framedata.h"
#include "renderer/light.h"
#include "renderer/updatesceneflag.h"

#include "terrain/iterraingenerator.h"
#include "terrain/terrainconfig.h"

#include "exporter/exporter.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <memory>

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
	void ExportTerrain(const FileType& type, const std::string& path);
	void RebuildTerrainGenerator();
	void positionAndOrientateCamera(ICamera& camera);

	FrameData getFrameData(const ICamera& camera);
	TerrainConfig& getConfig();
	TerrainMesh& getTerrainMesh(); //Needs Refactoring Heightmap Panel in Editor Directly Calls
	Light& getLight(); //Needs Refactoring Light Panel in Editor Directly Calls

private:
	TerrainConfig config;
	std::unique_ptr<ITerrainGenerator> terrainGenerator;
	TerrainMesh terrainMesh;
	Transform& terrainTransform;

	Skybox skybox;
	SkyboxMesh skyboxMesh;

	Light light;

	uint8_t flags;

	bool generated;
};

#endif