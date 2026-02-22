#include <utility>
#include <iostream>

#include "core/graphicscontext.h"
#include "core/application.h"

#include "core/window.h"
#include "core/camera.h"

#include "UI/imguilayer.h"
#include "UI/uibase.h"

#include "renderer/scene.h"
#include "renderer/renderer.h"
#include "renderer/shader.h"

int main()
{
    try
    {
		// Initialize Window and Graphics Context
		std::unique_ptr<Window> window = std::make_unique<Window>(
			SCR_WIDTH,
			SCR_HEIGHT,
			"Terrain Generator",
			true
		);

		GraphicsContext graphics(*window);
		graphics.Init();

		// Initialize Camera
		std::unique_ptr<Camera> camera = std::make_unique<Camera>(
			SCR_WIDTH,
			SCR_HEIGHT,
			WORLD_ORIGIN
		);

		// Initialize Scene
		std::unique_ptr<Scene> scene = std::make_unique<Scene>();

		// Initialize Shaders and Renderer
		std::unique_ptr<Shader> depthShader = std::make_unique<Shader>();
		std::unique_ptr<Shader> terrainShader = std::make_unique<Shader>();
		std::unique_ptr<Shader> skyboxShader = std::make_unique<Shader>();

		terrainShader->Load("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
		depthShader->Load("assets/shaders/depthVertex.glsl", "assets/shaders/depthFragment.glsl");
		skyboxShader->Load("assets/shaders/skyboxVertex.glsl", "assets/shaders/skyboxFragment.glsl");

		std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(
			std::move(depthShader),
			std::move(terrainShader),
			std::move(skyboxShader),
			SCR_WIDTH,
			SCR_HEIGHT
		);

		// Initialize ImGUI
		std::unique_ptr<ImGuiLayer> imguiLayer = std::make_unique<ImGuiLayer>();

		// Initialize User Interface
		std::unique_ptr<UIBase> ui = std::make_unique<UIBase>(
			*scene,
			*camera,
			*renderer
		);

        Application application(
            std::move(window),
            std::move(camera),
            std::move(scene),
            std::move(renderer),
            std::move(imguiLayer),
            std::move(ui)
        );
        application.Run();
    }
	catch (const std::exception ex)
    {
        std::cerr << "An Error Occurred during Application Runtime: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}