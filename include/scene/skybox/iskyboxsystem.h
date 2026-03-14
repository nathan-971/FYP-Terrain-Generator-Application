#ifndef _I_SKYBOX_SYSTEM_H_
#define _I_SKYBOX_SYSTEM_H_

#include "scene/skybox/skyboxoption.h"
#include "scene/skybox/skyboxmesh.h"

class ISkyboxSystem
{
public:
	virtual ~ISkyboxSystem() = default;
	virtual void LoadTextures() = 0;
	virtual bool Change(SkyboxOption option) = 0;
	virtual bool isDisabled() = 0;

	virtual unsigned int getActiveTexture() = 0;
	virtual SkyboxMesh& getMesh() = 0;
};

#endif