#include "scene/skybox/skyboxsystem.h"

SkyboxSystem::SkyboxSystem() 
	: skybox(), mesh() { }

SkyboxSystem::~SkyboxSystem() { }

void SkyboxSystem::LoadTextures()
{
	skybox.LoadTextures();
}

bool SkyboxSystem::Change(SkyboxOption option)
{
	return skybox.Change(option);
}

bool SkyboxSystem::isDisabled()
{
	return skybox.isDisabled();
}

unsigned int SkyboxSystem::getActiveTexture()
{
	return skybox.getActiveTextureId();
}

SkyboxMesh& SkyboxSystem::getMesh()
{
	return mesh;
}