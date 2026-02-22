#include "UI/panels/lightingpanel.h"

void LightingPanel::Display(EditorContext& ctx)
{
    ImGui::SeparatorText("Lighting Configuration");
    ImGui::SliderFloat3("Light Position", &ctx.scene.getLight().position.x, -150.0f, 150.0f);
    ImGui::SliderFloat3("Light Color", &ctx.scene.getLight().color.x, 0.0f, 1.0f);
    ImGui::SliderFloat("Ambient Strength", &ctx.scene.getLight().ambient, 0.0f, 1.0f);
    ImGui::SliderInt("Shininess", &ctx.scene.getLight().shininess, 2, 256);
    ImGui::SliderFloat("Specular Strength", &ctx.scene.getLight().specular, 0.0f, 1.0f);
}