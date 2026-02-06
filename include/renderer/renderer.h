#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "core/window.h"
#include "core/camera.h"

#include "renderer/shader.h"
#include "renderer/framedata.h"
#include "renderer/scene.h"

#include <glm/glm.hpp>

class Renderer 
{
public:
	Renderer();
	~Renderer();

	void RenderScene(Window& window, Camera& camera, Scene& scene);

private:
	void renderDepthPass(Scene& scene);
	void renderLightPass(Scene& scene, FrameData& frameData);
	void renderSkyboxPass(Scene& scene, FrameData& frameData);

	void generateShadowMap();

	Shader terrainShader;
	Shader depthShader;
	Shader skyboxShader;

	unsigned int shadowMap;
	unsigned int shadowMapFBO;
	int shadowMapWidth;
	int shadowMapHeight;

	glm::mat4 orthogonalProjection;
};

#endif