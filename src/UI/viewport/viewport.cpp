#include "ui/viewport/viewport.h"

Viewport::Viewport(Scene& scene, Camera& camera, IViewportProvider& viewportProvider) :
	scene(scene), 
	camera(camera),
    viewportProvider(viewportProvider) { }

Viewport::~Viewport() { }

void Viewport::Render()
{
    ImVec2 avail = ImGui::GetContentRegionAvail();
    ImGui::Image(
        (ImTextureID)(int*)viewportProvider.getViewportTexture(),
        avail,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::End();
}

void Viewport::UpdateSize()
{
    ImGui::Begin("Terrain Viewport");
    ImVec2 avail = ImGui::GetContentRegionAvail();
    int newWidth = static_cast<int>(avail.x);
    int newHeight = static_cast<int>(avail.y);

    if (newWidth != viewportProvider.getViewportWidth() || newHeight != viewportProvider.getViewportHeight())
    {
        viewportProvider.ResizeViewport(newWidth, newHeight);
        camera.OnResize(newWidth, newHeight);
    }
}