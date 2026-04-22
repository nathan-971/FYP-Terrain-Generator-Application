#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "scene/iscene.h"

#include "renderer/irenderer.h"
#include "renderer/iviewportprovider.h"
#include "renderer/ibakedalbedoprovider.h"
#include "renderer/ishader.h"
#include "renderer/framedata.h"

#include "ui/viewport/framebuffer.h"

#include <memory>
#include <glm/glm.hpp>

class Renderer : public IRenderer, public IViewportProvider, public IBakedAlbedoProvider
{
public:
	Renderer(
		std::unique_ptr<IShader> depthShader,
		std::unique_ptr<IShader> terrainShader,
		std::unique_ptr<IShader> skyboxShader,
		std::unique_ptr<IShader> bakeAlbedoShader,
		int initialWidth,
		int initiaHeight
	);
	~Renderer();

	void RenderScene(const FrameData& frameData) override;
	FrameData getFrameData(const ICamera& camera, const IScene& scene) override;

	void ResizeViewport(int width, int height) override;
	unsigned int getViewportTexture() const override;
	int getViewportWidth() const override;
	int getViewportHeight() const override;

	void RenderFinishedAlbedoToTexture(const IScene& scene) override;
	unsigned int getBakedAlbedoTexture() const override;
	int getBakedAlbedoWidth() const override;
	int getBakedAlbedoHeight() const override;

private:
	void renderDepthPass(const FrameData& frameData);
	void renderLightPass(const FrameData& frameData);
	void renderSkyboxPass(const FrameData& frameData);

	void createShadowFrameBuffer(int width, int height);
	void createViewportFrameBuffer(int width, int height);
	void createBakeAlbedoFrameBuffer(int width, int height);

	void destroyFramebuffer(Framebuffer& framebuffer);

	std::unique_ptr<IShader> depthShader;
	std::unique_ptr<IShader> terrainShader;
	std::unique_ptr<IShader> skyboxShader;
	std::unique_ptr<IShader> bakeAlbedoShader;

	Framebuffer shadowMapFramebuffer;
	Framebuffer viewportFramebuffer;
	Framebuffer bakeAlbedoFrameBuffer;
};

#endif