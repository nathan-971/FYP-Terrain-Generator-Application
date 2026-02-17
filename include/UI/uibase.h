#ifndef _UI_BASE_H_
#define _UI_BASE_H_

#include "core/camera.h"

#include "UI/editor/editor.h"
#include "UI/viewport/viewport.h"

#include "renderer/scene.h"
#include "renderer/renderer.h"

class UIBase
{
public:
	UIBase(Scene& scene, Camera& camera, Renderer& renderer);
	~UIBase();

	void Render();

private:
	void StartRootFrame();
	void LayoutRootFrame();

	Scene& scene;
	Camera& camera;
	Renderer& renderer;

	Viewport viewport;
	Editor editor;
};

#endif