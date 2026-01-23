#ifndef _SCENE_H_
#define _SCENE_H_

#define WORLD_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

#include "core/camera.h"
#include "core/window.h"
#include "renderer/shader.h"
#include "renderer/terrainmesh.h"
#include "renderer/skybox.h"
#include "renderer/skyboxmesh.h"
#include "terrain/terraingenerator.h"
#include "terrain/terrainconfig.h"

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

	void Render(Window& window, Camera& camera);
	void FlagForUpdate(UpdateSceneFlag flag);
	void Update();
	void Generate();
	bool isGenerated() const;
	bool ChangeSkybox(SkyboxOption option);
	void exportTerrain();

	TerrainConfig& getTerrainConfig();

	//Temporary Lightning Values
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	float ambientStrength;
	float specularStrength;
	int shininess;

	Shader terrainShader;
private:
	void renderDepthPass(glm::mat4& lightSpaceMatrix);
	void renderScenePass(glm::mat4& lightSpaceMatrix, Camera& camera);
	void renderSkyboxPass(Camera& camera);
	void generateShadowMap();

	TerrainConfig config;
	TerrainGenerator terrainGenerator;
	TerrainMesh terrainMesh;
	Shader depthShader;

	Skybox skybox;
	SkyboxMesh skyboxMesh;
	Shader skyboxShader;

	uint8_t flags;

	//Shadow Map Variables
	unsigned int shadowMapWidth;
	unsigned int shadowMapHeight;
	unsigned int shadowMap;
	unsigned int shadowMapFBO;

	bool generated;

	glm::mat4 orthgonalProjection;
};

#endif