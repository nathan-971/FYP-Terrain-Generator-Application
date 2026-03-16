#ifndef _I_SKYBOX_SYSTEM_H_
#define _I_SKYBOX_SYSTEM_H_

#include "scene/skybox/skyboxconfig.h"
#include "scene/skybox/skyboxoption.h"
#include "scene/skybox/skyboxmesh.h"

class ISkyboxSystem
{
public:
	virtual ~ISkyboxSystem() = default;
	virtual void LoadTextures() = 0;
	virtual bool Change() = 0;
	virtual const bool isDisabled() const = 0;

	virtual SkyboxConfig& getConfig() = 0;
	virtual const unsigned int getActiveTexture() const = 0;
	virtual SkyboxMesh& getMesh() = 0;
	virtual const SkyboxMesh& getMesh() const = 0;
};

#endif