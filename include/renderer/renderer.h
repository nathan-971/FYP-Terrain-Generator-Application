#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <memory>
#include <glm/glm.hpp>

#include "renderer/irenderer.h"
#include "renderer/iviewportprovider.h"
#include "renderer/ishader.h"
#include "renderer/framedata.h"
#include "ui/viewport/framebuffer.h"

class Renderer : public IRenderer, public IViewportProvider
{
public:
	Renderer(
		std::unique_ptr<IShader> depthShader,
		std::unique_ptr<IShader> terrainShader,
		std::unique_ptr<IShader> skyboxShader,
		int initialWidth,
		int initiaHeight
	);
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

	std::unique_ptr<IShader> depthShader;
	std::unique_ptr<IShader> terrainShader;
	std::unique_ptr<IShader> skyboxShader;

	Framebuffer shadowMapFramebuffer;
	Framebuffer viewportFramebuffer;
};

#endif