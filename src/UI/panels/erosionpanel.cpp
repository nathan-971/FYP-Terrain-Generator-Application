#include "UI/panels/erosionpanel.h"

void ErosionPanel::Display(EditorContext& ctx)
{
    auto& config = ctx.terrainConfig;
    auto& commands = ctx.commands;
    auto& state = ctx.state;

    ImGui::SeparatorText("Erosion Settings");
    if (ImGui::Checkbox("Enable Erosion", &state.erosionEnabled))
    {
		commands.changeErosionEnabled = true;
		commands.updateHeightMap = true;
    }

    if (ImGui::Button("Start Eroding"))
    {

    }
    ImGui::SameLine();
    if (ImGui::Button("Stop Eroding"))
    {

    }
    ImGui::SameLine();
    ImGui::Text("Number of Drops:");
}