#ifndef _I_SCENE_H_
#define _I_SCENE_H_

#include "scene/updatesceneflag.h"
#include "scene/terrain/iterrainsystem.h"
#include "scene/skybox/iskyboxsystem.h"
#include "scene/lighting/ilightingsystem.h"

class IScene
{
public:
	virtual ~IScene() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void FlagForUpdate(UpdateSceneFlag flag) = 0;

	virtual ITerrainSystem& Terrain() = 0;
	virtual const ITerrainSystem& Terrain() const = 0;

	virtual ISkyboxSystem& Skybox() = 0;
	virtual const ISkyboxSystem& Skybox() const = 0;

	virtual ILightingSystem& Lighting() = 0;
	virtual const ILightingSystem& Lighting() const = 0;
};

#endif