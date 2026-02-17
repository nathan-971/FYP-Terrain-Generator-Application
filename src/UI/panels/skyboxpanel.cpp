#include "UI/panels/skyboxpanel.h"

void SkyboxPanel::Display(EditorContext& ctx)
{
	auto& state = ctx.state;
    auto& commands = ctx.commands;

    ImGui::SeparatorText("Skybox Settings");
    if (ImGui::Button(ICON_FA_CLOUD_SUN, ImVec2(30, 30)))
    {
		commands.changeSkybox = true;
		state.skyboxOption = SkyboxOption::MORNING;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_SUN, ImVec2(30, 30)))
    {
        commands.changeSkybox = true;
		state.skyboxOption = SkyboxOption::NOON;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_MOON, ImVec2(30, 30)))
    {
        commands.changeSkybox = true;
        state.skyboxOption = SkyboxOption::NIGHT;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_CIRCLE_XMARK, ImVec2(30, 30)))
    {
        commands.changeSkybox = true;
		state.skyboxOption = SkyboxOption::NONE;
    }
}