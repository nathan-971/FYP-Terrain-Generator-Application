#ifndef _SKYBOX_SYSTEM_H_
#define _SKYBOX_SYSTEM_H_

#include "scene/skybox/iskyboxsystem.h"
#include "scene/skybox/skyboxconfig.h"
#include "scene/skybox/skyboxoption.h"
#include "scene/skybox/skybox.h"
#include "scene/skybox/skyboxmesh.h"

class SkyboxSystem : public ISkyboxSystem
{
public:
	SkyboxSystem();
	~SkyboxSystem();

	void LoadTextures() override;
	bool Change() override;
	bool const isDisabled() const override;
	const unsigned int getActiveTexture() const override;
	SkyboxConfig& getConfig();

	SkyboxMesh& getMesh() override;
	const SkyboxMesh& getMesh() const override;

private:
	SkyboxConfig config;
	Skybox skybox;
	SkyboxMesh mesh;
};

#endif