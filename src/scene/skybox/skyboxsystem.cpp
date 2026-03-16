#include "scene/skybox/skyboxsystem.h"

SkyboxSystem::SkyboxSystem() 
	: skybox(), mesh() { }

SkyboxSystem::~SkyboxSystem() { }

void SkyboxSystem::LoadTextures()
{
	skybox.LoadTextures();
}

bool SkyboxSystem::Change()
{
	return skybox.Change(config.skyboxOption);
}

const bool SkyboxSystem::isDisabled() const
{
	return skybox.isDisabled();
}

const unsigned int SkyboxSystem::getActiveTexture() const
{
	return skybox.getActiveTextureId();
}

SkyboxMesh& SkyboxSystem::getMesh()
{
	return mesh;
}

const SkyboxMesh& SkyboxSystem::getMesh() const
{
	return mesh;
}

SkyboxConfig& SkyboxSystem::getConfig()
{
	return config;
}