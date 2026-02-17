#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#define FOV 75.0f
#define NEAR_PLANE 0.05f
#define FAR_PLANE 250.0f

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "core/camera.h"
#include "ui/viewport/framebuffer.h"

#include "renderer/renderer.h"
#include "renderer/scene.h"

class Viewport
{
public:
	Viewport(Scene& scene, Camera& camera, Renderer& renderer);
	~Viewport();

	void Render();

private:
	void Resize(int width, int height);
	void RenderSceneToTexture();

	Framebuffer framebuffer;
	Scene& scene;
	Camera& camera;
	Renderer& renderer;
};

#endif