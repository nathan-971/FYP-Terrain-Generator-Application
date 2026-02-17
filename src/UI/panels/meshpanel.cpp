#include "UI/panels/meshpanel.h"

void MeshPanel::Display(EditorContext& ctx)
{
    auto& config = ctx.terrainConfig;
	auto& commands = ctx.commands;

    ImGui::SeparatorText("Mesh Configuration");
    if (ImGui::SliderInt("Terrain Width", (int*)&config.width, TERRAIN_MIN_WIDTH, TERRAIN_MAX_WIDTH))
    {
        commands.updateMesh = true;
        commands.updateHeightMap = true;
    }

    if (ImGui::SliderInt("Terrain Depth", (int*)&config.depth, TERRAIN_MIN_DEPTH, TERRAIN_MAX_DEPTH))
    {
        commands.updateMesh = true;
        commands.updateHeightMap = true;
    }
    if (ImGui::SliderFloat("Terrain Resolution", (float*)&config.resolution, 1.0f, 0.5f))
    {
        commands.updateMesh = true;
        commands.updateHeightMap = true;
    }
    ImGui::SliderFloat("Rotation Speed", (float*)&config.rotationSpeed, 10.0f, 100.0f);
}