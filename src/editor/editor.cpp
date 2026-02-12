#include "editor/editor.h"

Editor::Editor(Scene& scene, Camera& camera) : 
	scene(scene), 
	camera(camera),
	seed(0),
	noiseConfig(NoiseConfiguration::BaseNoise),
	warpMode(WarpMode::None) { }

//TEMPORARY
const char* noiseConfigNames[] = { "Base Noise", "Ridged Noise" };
static NoiseConfiguration selectedConfig = NoiseConfiguration::BaseNoise;

//TEMPORARY
const char* warpModeNames[] = { "None", "Single", "Double" };
static WarpMode selectedWarpMode = WarpMode::None;

void Editor::Update()
{
	ImGui::Begin("Terrain Settings");
    ImGui::Text(
        "Camera Position: ( %.2f, %.2f, %.2f )",
        camera.getPosition().x,
        camera.getPosition().y,
        camera.getPosition().z
    );
}

void Editor::Render()
{
	ImGui::Begin("Terrain Editor");

    TerrainConfig& config = scene.getTerrainConfig();
    drawMeshPanel(config);
    drawNoisePanel(config);
    drawLightingPanel();
    drawErosionPanel(config);
    drawSkyboxPanel();
    drawExportPanel();

	ImGui::End();
}

void Editor::drawMeshPanel(TerrainConfig& config)
{
    ImGui::SeparatorText("Mesh Configuration");
    if (ImGui::SliderInt("Terrain Width", (int*)&config.width, TERRAIN_MIN_WIDTH, TERRAIN_MAX_WIDTH))
    {
        scene.FlagForUpdate(UpdateSceneFlag::Mesh);
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    if (ImGui::SliderInt("Terrain Depth", (int*)&config.depth, TERRAIN_MIN_DEPTH, TERRAIN_MAX_DEPTH))
    {
        scene.FlagForUpdate(UpdateSceneFlag::Mesh);
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }
    if (ImGui::SliderFloat("Terrain Resolution", (float*)&config.resolution, 1.0f, 0.5f))
    {
        scene.FlagForUpdate(UpdateSceneFlag::Mesh);
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }
    ImGui::SliderFloat("Rotation Speed", (float*)&config.rotationSpeed, 10.0f, 100.0f);
}

void Editor::drawNoisePanel(TerrainConfig& config)
{
    ImGui::SeparatorText("Noise Settings");
    if (ImGui::BeginCombo("Noise Configuration", noiseConfigNames[static_cast<int>(selectedConfig)]))
    {
        for (int i = 0; i < sizeof(noiseConfigNames) / sizeof(noiseConfigNames[0]); ++i)
        {
            bool isSelected = (selectedConfig == static_cast<NoiseConfiguration>(i));
            if (ImGui::Selectable(noiseConfigNames[i], isSelected))
            {
                selectedConfig = static_cast<NoiseConfiguration>(i);

                scene.getTerrainGenerator().setNoiseConfiguration(selectedConfig);
                scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::SliderInt("Noise Octaves", static_cast<int*>(&config.octaves), 1, 8))
    {
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    if (ImGui::SliderFloat("Noise Amplitude", static_cast<float*>(&config.amplitude), 1.0f, 15.0f))
    {
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    if (ImGui::SliderFloat("Noise Frequency", static_cast<float*>(&config.frequency), 0.01f, 0.5f))
    {
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    if (ImGui::SliderFloat("Noise Lacunarity", static_cast<float*>(&config.lacunarity), 1.0f, 2.0f))
    {
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    if (ImGui::SliderFloat("Noise Persistence", static_cast<float*>(&config.persistence), 0.01f, 0.75f))
    {
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    if (ImGui::SliderFloat("Noise Scale", static_cast<float*>(&config.scale), 0.1f, 2.5f))
    {
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    if (ImGui::BeginCombo("Warp Configuration", warpModeNames[static_cast<int>(selectedWarpMode)]))
    {
        for (int i = 0; i < sizeof(warpModeNames) / sizeof(warpModeNames[0]); ++i)
        {
            bool isSelected = (selectedWarpMode == static_cast<WarpMode>(i));
            if (ImGui::Selectable(warpModeNames[i], isSelected))
            {
                selectedWarpMode = static_cast<WarpMode>(i);

                scene.getTerrainGenerator().setWarpMode(selectedWarpMode);
                scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::SliderFloat("Wrap Multiplier", static_cast<float*>(&config.warpMultiplier), 0.0f, 200.0f))
    {
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    if (ImGui::SliderFloat("Wrap Freqeuncy", static_cast<float*>(&config.warpFrequency), 0.000f, 0.03f))
    {
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }

    ImGui::InputInt("Seed", &seed, ImGuiInputTextFlags_ReadOnly);
    if (ImGui::Button("Generate"))
    {
        seed = generateSeed();
        scene.getTerrainGenerator().setSeed(seed);
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }
}

void Editor::drawLightingPanel()
{
    ImGui::SeparatorText("Lighting Configuration");
    ImGui::SliderFloat3("Light Position", &scene.getLight().position.x, 0.0f, 100.0f);
    ImGui::SliderFloat3("Light Color", &scene.getLight().color.x, 0.0f, 1.0f);
    ImGui::SliderFloat("Ambient Strength", &scene.getLight().ambient, 0.0f, 1.0f);
    ImGui::SliderInt("Shininess", &scene.getLight().shininess, 2, 256);
    ImGui::SliderFloat("Specular Strength", &scene.getLight().specular, 0.0f, 1.0f);
}

void Editor::drawErosionPanel(TerrainConfig& config)
{
    ImGui::SeparatorText("Erosion Settings");
    if (ImGui::Checkbox("Enable Erosion", &config.enableErosion))
    {
        scene.getTerrainGenerator().toggleErosion();
        scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
    }
}

void Editor::drawSkyboxPanel()
{
    ImGui::SeparatorText("Skybox Settings");
    if (ImGui::Button(ICON_FA_CLOUD_SUN, ImVec2(30, 30)))
    {
        scene.ChangeSkybox(SkyboxOption::MORNING);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_SUN, ImVec2(30, 30)))
    {
        scene.ChangeSkybox(SkyboxOption::NOON);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_MOON, ImVec2(30, 30)))
    {
        scene.ChangeSkybox(SkyboxOption::NIGHT);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_CIRCLE_XMARK, ImVec2(30, 30)))
    {
        scene.ChangeSkybox(SkyboxOption::NONE);
    }
}

void Editor::drawExportPanel()
{
    ImGui::SeparatorText("Export Terrain");
    if (ImGui::Button(ICON_FA_FLOPPY_DISK, ImVec2(30 , 30)))
    {
        scene.ExportTerrain(FileType::FBX);
    }
    ImGui::SameLine();
    ImGui::Text("Save Terrain");
}

int Editor::generateSeed()
{
    std::random_device rngDevice;
    std::mt19937 gen(rngDevice());
    std::uniform_int_distribution<int> range(100000000, 999999999); //Seed is 9 Numbers

    return range(gen);
}