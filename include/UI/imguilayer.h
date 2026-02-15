#ifndef _IMGUI_LAYER_H_
#define _IMGUI_LAYER_H_

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "IconsFontAwesome6.h"

#include "core/window.h"

class ImGuiLayer
{
public:
	void Init(Window& window);
	void BeginFrame();
	void EndFrame();
	void Shutdown();
};

#endif