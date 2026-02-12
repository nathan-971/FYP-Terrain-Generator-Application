#include "core/application.h"

Application::Application() : 
	window(nullptr), 
	camera(nullptr), 
	scene(nullptr),
	renderer(nullptr),
	imguiLayer(nullptr),
	editor(nullptr)
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
	editor = std::make_unique<Editor>(
		*scene,
		*camera
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
		Update();
		Render();
		window->swapBuffers();
	}
}

void Application::Update()
{
	camera->updateCameraMatrix(FOV, NEAR_PLANE, FAR_PLANE);
	scene->Update();
}

void Application::Render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer->RenderScene(*window, *camera, *scene);

	imguiLayer->BeginFrame();
	editor->Render();
	imguiLayer->EndFrame();
}

void Application::Shutdown()
{
	if (imguiLayer)
	{
		imguiLayer->Shutdown();
	}
}