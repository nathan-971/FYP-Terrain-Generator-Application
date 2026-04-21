#include "UI/panels/erosionpanel.h"

void ErosionPanel::Display(EditorContext& ctx)
{
    auto& config = ctx.terrainConfig;
    auto& commands = ctx.commands;
    auto& state = ctx.state;

    ImGui::SeparatorText("Erosion Settings");
    if (ImGui::Checkbox("Enable Erosion", &state.erosionEnabled))
    {
        state.erosionRunning = false;
		commands.changeErosionEnabled = true;
		commands.updateHeightMap = true;
    }

    ImGui::Text("Status: ");
    ImGui::SameLine();
    if (state.erosionEnabled && state.erosionRunning)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 160, 0, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 160, 0, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 160, 0, 255));
        ImGui::Button("Running");
        ImGui::PopStyleColor(3);
    }
    else 
    {
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(160, 0, 0, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(160, 0, 0, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(160, 0, 0, 255));
        ImGui::Button("Stopped");
        ImGui::PopStyleColor(3);
    }

    ImGui::BeginDisabled(!state.erosionEnabled);
    if (ImGui::Button("Start Eroding"))
    {
        commands.startErosion = true;
        state.erosionRunning = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop Eroding"))
    {
        commands.stopErosion = true;
        state.erosionRunning = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Erosion"))
    {
        commands.resetErosion = true;
        state.erosionRunning = false;
    }
    ImGui::EndDisabled();
}