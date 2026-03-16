#include "scene/lighting/lightingsystem.h"

LightingSystem::LightingSystem() : light()
{ 
	light.position = glm::vec3(-150.0f, 150.0f, -150.0f);
	light.color = glm::vec3(1.0f);
	light.ambient = 0.1f;
	light.specular = 0.1f;
	light.shininess = 15;
}

LightingSystem::~LightingSystem() { }

void LightingSystem::setLightPositon(glm::vec3 position)
{
	light.position = position;
}

void LightingSystem::setLightColor(glm::vec3 color)
{
	light.color = color;
}

void LightingSystem::setLightAmbient(float ambient) 
{
	light.ambient = ambient;
}

void LightingSystem::setLightSpecular(float specular)
{
	light.specular = specular;
}

void LightingSystem::setLightShininess(int shininess)
{
	light.shininess = shininess;
}

Light& LightingSystem::getLight()
{
	return light;
}

const Light& LightingSystem::getLight() const
{
	return light;
}