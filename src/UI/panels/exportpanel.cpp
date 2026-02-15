#include "UI/panels/exportpanel.h"

void ExportPanel::Display(EditorContext& ctx)
{
    ImGui::SeparatorText("Export Terrain");
    if (ImGui::Button(ICON_FA_FLOPPY_DISK, ImVec2(30, 30)))
    {
        ctx.scene.ExportTerrain(FileType::FBX);
    }
    ImGui::SameLine();
    ImGui::Text("Save Terrain");
}