#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "core/camera.h"
#include "ui/viewport/framebuffer.h"

#include "renderer/renderer.h"
#include "renderer/scene.h"

class Viewport
{
public:
	Viewport(Scene& scene, Camera& camera, IViewportProvider& viewportProvider);
	~Viewport();

	void Render();
	void UpdateSize();

private:
	Scene& scene;
	Camera& camera;
	IViewportProvider& viewportProvider;
};

#endif