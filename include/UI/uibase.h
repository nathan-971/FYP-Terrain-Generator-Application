#ifndef _UI_BASE_H_
#define _UI_BASE_H_

#include "core/icamera.h"

#include "UI/editor/editor.h"
#include "UI/viewport/viewport.h"

#include "scene/scene.h"

#include "renderer/iviewportprovider.h"

class UIBase
{
public:
	UIBase(Scene& scene, ICamera& camera, IViewportProvider& viewportProvider);
	~UIBase();

	void PreRender();
	void Render();

private:
	void StartRootFrame();
	void LayoutRootFrame();

	Scene& scene;
	ICamera& camera;
	IViewportProvider& viewportProvider;

	Viewport viewport;
	Editor editor;
};

#endif