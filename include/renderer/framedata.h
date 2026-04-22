#ifndef _FRAME_DATA_H_
#define _FRAME_DATA_H_

#include "scene/texture/material.h"

#include "scene/terrain/terrainmesh.h"

#include "scene/skybox/skyboxmesh.h"

#include <glm/glm.hpp>

struct TerrainRenderData
{
	glm::mat4 modelMatrix { };
	const TerrainMesh* terrainMesh;
	const Material* grass;
	const Material* stone;
};

struct SkyboxRenderData
{
	const SkyboxMesh* skyboxMesh;
	unsigned int skyboxTexture = 0;
	bool enabled = false;
};

struct FrameData 
{
	glm::mat4 viewMatrix { };
	glm::mat4 projectionMatrix { };

	glm::vec3 cameraPosition { };
	glm::vec3 cameraOrientation { };
	glm::vec3 cameraUp { };

	glm::mat4 lightSpaceMatrix { };
	glm::vec3 lightPosition { };
	glm::vec3 lightColor { };

	float ambientStrength = 0;
	float specularStrength = 0;
	int shininess = 0;

	TerrainRenderData terrain;
	SkyboxRenderData skybox;
};

#endif