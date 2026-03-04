#include "UI/editor/editor.h"
#include "UI/panels/meshpanel.h"
#include "UI/panels/noisepanel.h"
#include "UI/panels/lightingpanel.h"
#include "UI/panels/erosionpanel.h"
#include "UI/panels/skyboxpanel.h"
#include "UI/panels/exportpanel.h"
#include "UI/panels/heightmappanel.h"
#include "renderer/updatesceneflag.h"
#include "IconsFontAwesome6.h"

#include <iostream>
#include <filesystem>
#include <vector>

Editor::Editor(Scene& scene) : 
	scene(scene),
    ctx{ scene, scene.getConfig() }
{
    panels.push_back(std::make_unique<MeshPanel>());
    panels.push_back(std::make_unique<NoisePanel>());
    panels.push_back(std::make_unique<LightingPanel>());
    panels.push_back(std::make_unique<ErosionPanel>());
    panels.push_back(std::make_unique<HeightMapPanel>());
    panels.push_back(std::make_unique<SkyboxPanel>());
    panels.push_back(std::make_unique<ExportPanel>());
}

void Editor::Render()
{
	ImGui::Begin("Terrain Editor");
    for (std::unique_ptr<IPanel>& panel : panels)
    {
        panel->Display(ctx);
    }
	ImGui::End();
    renderExportPopup();
    renderFileDialog();
}

void Editor::ApplyCommands()
{
    if (ctx.commands.changeWarpMode)
    {
        ctx.terrainConfig.warpMode = ctx.state.warpMode;
        scene.FlagForUpdate(UpdateSceneFlag::RebuildTerrainGenerator);
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
        ctx.commands.changeWarpMode = false;
    }

    if (ctx.commands.changeNoiseConfiguration)
    {
        ctx.terrainConfig.noiseConfig = ctx.state.noiseConfig;
        scene.FlagForUpdate(UpdateSceneFlag::RebuildTerrainGenerator);
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
        ctx.commands.changeNoiseConfiguration = false;
    }

    if (ctx.commands.changeErosionEnabled)
    {
        ctx.terrainConfig.erosionEnabled = ctx.state.erosionEnabled;
        scene.FlagForUpdate(UpdateSceneFlag::RebuildTerrainGenerator);
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
        ctx.commands.changeErosionEnabled = false;
    }

    if (ctx.commands.changeSeed)
    {
        ctx.terrainConfig.seed = ctx.state.seed;
        scene.FlagForUpdate(UpdateSceneFlag::RebuildTerrainGenerator);
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
        ctx.commands.changeSeed = false;
    }

    if (ctx.commands.updateMesh)
    {
        scene.FlagForUpdate(UpdateSceneFlag::Mesh);
        ctx.commands.updateMesh = false;
    }

    if (ctx.commands.updateHeightMap)
    {
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
        ctx.commands.updateHeightMap = false;
    }

    if (ctx.commands.changeSkybox)
    {
		scene.ChangeSkybox(ctx.state.skyboxOption);
		ctx.commands.changeSkybox = false;
    }

    if (ctx.commands.requestExport)
    {
        showExportPopup = true;
        ImGui::OpenPopup("Choose Export File Type");
        ctx.commands.requestExport = false;
    }

    if (exportConfirmed)
    {
		ImGui::OpenPopup("Export File");
        exportConfirmed = false;
    }

    if (exportPathMade)
    {
        scene.ExportTerrain(selectedExportType, exportPathString);
        exportPathMade = false;
    }
}

void Editor::renderExportPopup()
{
    if (ImGui::BeginPopupModal("Choose Export File Type", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        const char* fileTypeNames[] = { "FBX", "OBJ" };
        ImGui::Text("Select the file type to export terrain as:");
        if (ImGui::BeginCombo("File Type", fileTypeNames[static_cast<int>(selectedExportType)]))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = (selectedExportType == static_cast<FileType>(i));
                if (ImGui::Selectable(fileTypeNames[i], isSelected))
                {
                    selectedExportType = static_cast<FileType>(i);
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Select", ImVec2(120, 0)))
        {
            exportConfirmed = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void Editor::renderFileDialog()
{
	static std::filesystem::path currentPath = std::filesystem::path("C:\\");
    static std::filesystem::path previousPath = currentPath;

    static char selectedDirectory[256] = "";
    static char fileName[256] = "";

	if (ImGui::BeginPopupModal("Export File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::BeginListBox("##File Dialog", ImVec2(720, 15 * ImGui::GetTextLineHeightWithSpacing())))
        {
            if (currentPath.has_parent_path())
            {
                if (ImGui::Selectable("..."))
                {
                    currentPath = currentPath.parent_path();
                }
            }

            for (auto& entry : std::filesystem::directory_iterator(currentPath, std::filesystem::directory_options::skip_permission_denied))
            {
                if (entry.is_directory())
                {
                    std::string name = entry.path().filename().string();
                    std::string label = std::string(ICON_FA_FOLDER) + " " + name;
                    if (ImGui::Selectable(label.c_str()))
                    {
                        if (canAccessDirectory(entry.path()))
                        {
                            currentPath = entry.path();
                        }
                    }
                }
            }
            ImGui::EndListBox();
        }
        std::string currentPathStr = currentPath.string();
        strcpy_s(selectedDirectory, currentPathStr.c_str());

        ImGui::Text("Selected Directory");
        ImGui::SameLine();
        ImGui::InputText("##Selected Directory", selectedDirectory, IM_ARRAYSIZE(selectedDirectory), ImGuiInputTextFlags_ReadOnly);
       
        ImGui::SameLine();
        
        ImGui::Text("File Name");
        ImGui::SameLine();
        ImGui::InputText("##File Name", fileName, IM_ARRAYSIZE(fileName));

        if (ImGui::Button("Export", ImVec2(120, 0)))
        {
            std::string exportDir = selectedDirectory;
            std::string exportFile = fileName;

            if (exportFile == "")
            {
                exportFile = "terrainMesh";
            }

            std::string exportPath = exportDir + '\\' + exportFile;
            std::cout << "EXPORT PATH: " + exportPath;

            exportPathString = std::filesystem::path(exportPath).string();
            exportPathString = exportPathString + (selectedExportType == FileType::FBX ? ".fbx" : ".obj");
            exportPathMade = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            exportPathString = "";
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

bool Editor::canAccessDirectory(const std::filesystem::path& path)
{
    std::error_code ec;
    std::filesystem::directory_iterator it(
        path,
        std::filesystem::directory_options::skip_permission_denied,
        ec
    );
    return !ec;
}