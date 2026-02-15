#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

#define FOV 75.0f
#define NEAR_PLANE 0.05f
#define FAR_PLANE 250.0f

#define WORLD_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

#include <memory>

#include "core/window.h"
#include "core/camera.h"

#include "UI/imguilayer.h"
#include "UI/editor.h"

#include "renderer/scene.h"
#include "renderer/renderer.h"

class Application
{
public:
	Application();
	~Application();
	void Run();

private:
	void Init();
	void Shutdown();
	void Update();
	void Render();

	std::unique_ptr<Window> window;
	std::unique_ptr<Scene> scene;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Camera> camera;

	std::unique_ptr<ImGuiLayer> imguiLayer;
	std::unique_ptr<Editor> editor;

	float lastFrameTime = 0.0f;
};

#endif