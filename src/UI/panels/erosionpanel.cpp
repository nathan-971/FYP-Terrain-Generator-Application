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

    ImGui::BeginDisabled(!state.erosionEnabled);
    if (ImGui::Button("Start Eroding"))
    {
        commands.startErosion = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop Eroding"))
    {
        commands.stopErosion = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Erosion"))
    {
        commands.resetErosion = true;
    }
    ImGui::EndDisabled();
}