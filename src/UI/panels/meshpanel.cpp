#include "UI/panels/meshpanel.h"

void MeshPanel::Display(EditorContext& ctx)
{
    auto& config = ctx.terrainConfig;
	auto& commands = ctx.commands;

    ImGui::SeparatorText("Mesh Configuration");

    int width = config.width;
    if (ImGui::SliderInt("Terrain Width", &width, 2, 100))
    {
        config.width = width;
        commands.updateMesh = true;
        commands.updateHeightMap = true;
    }

    int depth = config.depth;
    if (ImGui::SliderInt("Terrain Depth", &depth, 2, 100))
    {
        config.depth = depth;
        commands.updateMesh = true;
        commands.updateHeightMap = true;
    }

    float resolution = config.resolution;
    if (ImGui::SliderFloat("Terrain Resolution", &resolution, 1.0f, 0.5f))
    {
        config.resolution = resolution;
        commands.updateMesh = true;
        commands.updateHeightMap = true;
    }
    ImGui::SliderFloat("Rotation Speed", (float*)&config.rotationSpeed, 10.0f, 100.0f);
}