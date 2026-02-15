#include "UI/panels/meshpanel.h"

void MeshPanel::Display(EditorContext& ctx)
{
    auto& config = ctx.terrainConfig;

    ImGui::SeparatorText("Mesh Configuration");
    if (ImGui::SliderInt("Terrain Width", (int*)&config.width, TERRAIN_MIN_WIDTH, TERRAIN_MAX_WIDTH))
    {
        ctx.scene.FlagForUpdate(UpdateSceneFlag::Mesh);
        ctx.scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    if (ImGui::SliderInt("Terrain Depth", (int*)&config.depth, TERRAIN_MIN_DEPTH, TERRAIN_MAX_DEPTH))
    {
        ctx.scene.FlagForUpdate(UpdateSceneFlag::Mesh);
        ctx.scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }
    if (ImGui::SliderFloat("Terrain Resolution", (float*)&config.resolution, 1.0f, 0.5f))
    {
        ctx.scene.FlagForUpdate(UpdateSceneFlag::Mesh);
        ctx.scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }
    ImGui::SliderFloat("Rotation Speed", (float*)&config.rotationSpeed, 10.0f, 100.0f);
}