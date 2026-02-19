#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "core/window.h"
#include "core/camera.h"

#include "renderer/irenderer.h"
#include "renderer/iviewportprovider.h"
#include "renderer/shader.h"
#include "renderer/framedata.h"
#include "ui/viewport/framebuffer.h"
#include "renderer/scene.h"

#include <glm/glm.hpp>

class Renderer : public IRenderer, public IViewportProvider
{
public:
	Renderer();
	~Renderer();

	void RenderScene(const FrameData& frameData) override;

	void ResizeViewport(int width, int height) override;
	unsigned int getViewportTexture() const override;
	int getViewportWidth() const override;
	int getViewportHeight() const override;

private:
	void renderDepthPass(const FrameData& frameData);
	void renderLightPass(const FrameData& frameData);
	void renderSkyboxPass(const FrameData& frameData);

	void createShadowFrameBuffer(int width, int height);
	void createViewportFrameBuffer(int width, int height);

	void destroyFramebuffer(Framebuffer& framebuffer);

	Shader terrainShader;
	Shader depthShader;
	Shader skyboxShader;

	Framebuffer shadowMapFramebuffer;
	Framebuffer viewportFramebuffer;

	glm::mat4 lightProjection;
};

#endif