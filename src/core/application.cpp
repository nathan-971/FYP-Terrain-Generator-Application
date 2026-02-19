#include "core/application.h"
#include <utility>

Application::Application(
	std::unique_ptr<Window> window,
	std::unique_ptr<Camera> camera,
	std::unique_ptr<Scene> scene,
	std::unique_ptr<IRenderer> renderer,
	std::unique_ptr<ImGuiLayer> imguiLayer,
	std::unique_ptr<UIBase> ui
) :
	window(std::move(window)),
	scene(std::move(scene)),
	renderer(std::move(renderer)),
	camera(std::move(camera)),
	imguiLayer(std::move(imguiLayer)),
	ui(std::move(ui))
{ 
	Init();
}

Application::~Application()
{
	Shutdown();
}

void Application::Init()
{
	imguiLayer->Init(*window);
	lastFrameTime = window->getTime();
	scene->Generate();
	scene->positionAndOrientateCamera(*camera);
}

void Application::Run()
{
	while (!window->shouldClose())
	{
		float currentTime = window->getTime();
		Time::deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;
		Time::applicationTime = currentTime;

		window->pollEvents();

		camera->UpdateCameraMatrix(FOV, NEAR_PLANE, FAR_PLANE);
		scene->Update();

		imguiLayer->BeginFrame();

		ui->PreRender();
		renderer->RenderScene(scene->getFrameData(*camera));
		ui->Render();

		imguiLayer->EndFrame();

		window->swapBuffers();
	}
}

void Application::Shutdown()
{
	if (imguiLayer)
	{
		imguiLayer->Shutdown();
	}
}