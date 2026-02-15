#include "UI/panels/skyboxpanel.h"

void SkyboxPanel::Display(EditorContext& ctx)
{
    ImGui::SeparatorText("Skybox Settings");
    if (ImGui::Button(ICON_FA_CLOUD_SUN, ImVec2(30, 30)))
    {
        ctx.scene.ChangeSkybox(SkyboxOption::MORNING);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_SUN, ImVec2(30, 30)))
    {
        ctx.scene.ChangeSkybox(SkyboxOption::NOON);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_MOON, ImVec2(30, 30)))
    {
        ctx.scene.ChangeSkybox(SkyboxOption::NIGHT);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_CIRCLE_XMARK, ImVec2(30, 30)))
    {
        ctx.scene.ChangeSkybox(SkyboxOption::NONE);
    }
}