#ifndef _SKYBOX_SYSTEM_H_
#define _SKYBOX_SYSTEM_H_

#include "scene/skybox/iskyboxsystem.h"
#include "scene/skybox/skyboxoption.h"
#include "scene/skybox/skybox.h"
#include "scene/skybox/skyboxmesh.h"

class SkyboxSystem : public ISkyboxSystem
{
public:
	SkyboxSystem();
	~SkyboxSystem();

	void LoadTextures() override;
	bool Change(SkyboxOption option) override;
	bool isDisabled() override;
	unsigned int getActiveTexture() override;

	SkyboxMesh& getMesh() override;

private:
	Skybox skybox;
	SkyboxMesh mesh;
};

#endif