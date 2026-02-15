#include "UI/editor.h"
#include "UI/panels/meshpanel.h"
#include "UI/panels/noisepanel.h"
#include "UI/panels/lightingpanel.h"
#include "UI/panels/erosionpanel.h"
#include "UI/panels/skyboxpanel.h"
#include "UI/panels/exportpanel.h"

Editor::Editor(Scene& scene, Camera& camera) : 
	scene(scene),
	camera(camera),
    ctx{ scene, scene.getTerrainConfig() }
{
    panels.push_back(std::make_unique<MeshPanel>());
    panels.push_back(std::make_unique<NoisePanel>());
    panels.push_back(std::make_unique<LightingPanel>());
    panels.push_back(std::make_unique<ErosionPanel>());
    panels.push_back(std::make_unique<SkyboxPanel>());
    panels.push_back(std::make_unique<ExportPanel>());
}

void Editor::Update()
{
	ImGui::Begin("Terrain Settings");
    ImGui::Text(
        "Camera Position: ( %.2f, %.2f, %.2f )",
        camera.getPosition().x,
        camera.getPosition().y,
        camera.getPosition().z
    );
    ImGui::End();
}

void Editor::Render()
{
	ImGui::Begin("Terrain Editor");
    for (std::unique_ptr<IPanel>& panel : panels)
    {
        panel->Display(ctx);
    }
	ImGui::End();
}