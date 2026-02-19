#include <utility>
#include <iostream>

#include "core/application.h"

#include "core/window.h"
#include "core/camera.h"

#include "UI/imguilayer.h"
#include "UI/uibase.h"

#include "renderer/scene.h"
#include "renderer/renderer.h"

int main()
{
    try
    {
		std::unique_ptr<Window> window = std::make_unique<Window>(
			SCR_HEIGHT,
			SCR_WIDTH,
			"Terrain Generator",
			true
		);

		std::unique_ptr<Camera> camera = std::make_unique<Camera>(
			SCR_HEIGHT,
			SCR_WIDTH,
			WORLD_ORIGIN
		);

		std::unique_ptr<Scene> scene = std::make_unique<Scene>();
		std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
		std::unique_ptr<ImGuiLayer> imguiLayer = std::make_unique<ImGuiLayer>();

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
	catch (const std::exception& ex)
    {
        std::cerr << "An Error Occurred during Application Runtime: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}