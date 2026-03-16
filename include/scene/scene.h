#ifndef _SCENE_H_
#define _SCENE_H_

#define WORLD_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

#include "scene/iscene.h"
#include "scene/terrain/iterrainsystem.h"
#include "scene/skybox/iskyboxsystem.h"
#include "scene/lighting/ilightingsystem.h"
#include "scene/updatesceneflag.h"

#include <memory>

class Scene : public IScene
{
public:
	Scene(
		std::unique_ptr<ITerrainSystem> terrainSystem, 
		std::unique_ptr<ISkyboxSystem> skyboxSystem,
		std::unique_ptr<ILightingSystem> lightingSystem
	);
	~Scene();

	void Update(float deltaTime) override;
	void FlagForUpdate(UpdateSceneFlag flag) override;

	ITerrainSystem& Terrain() override;
	const ITerrainSystem& Terrain() const override;

	ISkyboxSystem& Skybox() override;
	const ISkyboxSystem& Skybox() const override;

	ILightingSystem& Lighting() override;
	const ILightingSystem& Lighting() const override;

private:
	std::unique_ptr<ISkyboxSystem> skybox;
	std::unique_ptr<ITerrainSystem> terrain;
	std::unique_ptr<ILightingSystem> lighting;
	
	uint8_t flags;
};

#endif