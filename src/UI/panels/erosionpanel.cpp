#include "UI/panels/erosionpanel.h"

void ErosionPanel::Display(EditorContext& ctx)
{
    auto& config = ctx.terrainConfig;

    ImGui::SeparatorText("Erosion Settings");
    if (ImGui::Checkbox("Enable Erosion", &config.enableErosion))
    {
        ctx.scene.getTerrainGenerator().toggleErosion();
        ctx.scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }
}