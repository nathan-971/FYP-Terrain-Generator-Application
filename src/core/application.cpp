#include "core/application.h"
#include <utility>

Application::Application(
	std::unique_ptr<IWindow> window,
	std::unique_ptr<ICamera> camera,
	std::unique_ptr<ICameraController> cameraController,
	std::unique_ptr<IScene> scene,
	std::unique_ptr<IRenderer> renderer,
	std::unique_ptr<ImGuiLayer> imguiLayer,
	std::unique_ptr<UIBase> ui
) :
	window(std::move(window)),
	camera(std::move(camera)),
	cameraController(std::move(cameraController)),
	scene(std::move(scene)),
	renderer(std::move(renderer)),
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
	cameraController->PositionCamera(*camera, scene->Terrain().getTerrainPosition());
}

void Application::Run()
{
	while (!window->shouldClose())
	{
		float currentTime = window->getTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		window->pollEvents();

		camera->UpdateCameraMatrix(FOV, NEAR_PLANE, FAR_PLANE);
		scene->Update(deltaTime);

		imguiLayer->BeginFrame();

		ui->PreRender();
		renderer->RenderScene(renderer->getFrameData(*camera, *scene));
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