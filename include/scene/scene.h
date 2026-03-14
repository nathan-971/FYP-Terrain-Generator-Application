#ifndef _SCENE_H_
#define _SCENE_H_

#define WORLD_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

#include "core/icamera.h"

#include "scene/terrain/terrainsystem.h"
#include "scene/skybox/iskyboxsystem.h"
#include "scene/lighting/light.h"
#include "scene/updatesceneflag.h"

#include "renderer/framedata.h"

#include "exporter/iexporter.h"

#include "utils/iexporterfactory.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <memory>

class Scene
{
public:
	Scene(
		std::unique_ptr<ITerrainSystem> terrainSystem, 
		std::unique_ptr<ISkyboxSystem> skyboxSystem,
		std::unique_ptr<IExporterFactory> exporterFactory
	);
	~Scene();

	void Generate();
	bool isGenerated() const;

	void Update(float deltaTime);
	void FlagForUpdate(UpdateSceneFlag flag);

	void ExportTerrain(const FileType& type, const std::string& path);
	void positionAndOrientateCamera(ICamera& camera);

	void StartErosion();
	void StopErosion();
	void ResetErosion();

	FrameData getFrameData(const ICamera& camera);
	TerrainConfig& getConfig();
	//TerrainMesh& getTerrainMesh(); //Needs Refactoring Heightmap Panel in Editor Directly Calls
	Light& getLight(); //Needs Refactoring Light Panel in Editor Directly Calls

private:
	std::unique_ptr<IExporterFactory> exporterFactory;
	std::unique_ptr<ISkyboxSystem> skybox;
	std::unique_ptr<ITerrainSystem> terrain;

	TerrainConfig config;

	Light light;
	
	uint8_t flags;

	bool generated;
};

#endif