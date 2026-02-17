#include "core/application.h"

Application::Application() : 
	window(nullptr), 
	camera(nullptr), 
	scene(nullptr),
	renderer(nullptr),
	imguiLayer(nullptr),
	ui(nullptr)
{ 
	Init();
}

Application::~Application()
{
	Shutdown();
}

void Application::Init()
{
	window = std::make_unique<Window>(
		SCR_HEIGHT, 
		SCR_WIDTH, 
		"Terrain Generator", 
		true
	);

	camera = std::make_unique<Camera>(
		SCR_HEIGHT,
		SCR_WIDTH,
		WORLD_ORIGIN
	);

	scene = std::make_unique<Scene>();
	renderer = std::make_unique<Renderer>();
	imguiLayer = std::make_unique<ImGuiLayer>();
	ui = std::make_unique<UIBase>(
		*scene,
		*camera,
		*renderer
	);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

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

		imguiLayer->BeginFrame();
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