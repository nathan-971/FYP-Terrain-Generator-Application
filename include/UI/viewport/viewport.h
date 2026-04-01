#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "core/camera/icamera.h"

#include "ui/viewport/framebuffer.h"

#include "renderer/iviewportprovider.h"

class Viewport
{
public:
	Viewport(ICamera& camera, IViewportProvider& viewportProvider);
	~Viewport();

	void Render();
	void UpdateSize();

private:
	ICamera& camera;
	IViewportProvider& viewportProvider;
};

#endif