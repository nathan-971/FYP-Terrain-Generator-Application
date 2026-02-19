#ifndef _FRAME_DATA_H_
#define _FRAME_DATA_H_

#include <glm/glm.hpp>

struct TerrainRenderData
{
	glm::mat4 modelMatrix;
	TerrainMesh* terrainMesh;
};

struct SkyboxRenderData
{
	SkyboxMesh* skyboxMesh;
	unsigned int skyboxTexture;
	bool enabled = false;
};

struct FrameData 
{
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 cameraPosition;
	glm::vec3 cameraOrientation;
	glm::vec3 cameraUp;

	glm::mat4 lightSpaceMatrix;
	glm::vec3 lightPosition;
	glm::vec3 lightColor;

	float ambientStrength;
	float specularStrength;
	int shininess;

	TerrainRenderData terrain;
	SkyboxRenderData skybox;
};

#endif