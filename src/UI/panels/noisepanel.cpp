#include "UI/panels/noisepanel.h"
#include "terrain/noise/noiseconfiguration.h"
#include "terrain/warp/warpmode.h"

const char* noiseConfigNames[] = { "Base Noise", "Ridged Noise" };
const char* warpModeNames[] = { "None", "Single", "Double" };

void NoisePanel::Display(EditorContext& ctx)
{
    auto& config = ctx.terrainConfig;
    auto& state = ctx.state;
	auto& commands = ctx.commands;

    ImGui::SeparatorText("Noise Settings");
    if (ImGui::BeginCombo("Noise Configuration", noiseConfigNames[static_cast<int>(state.noiseConfig)]))
    {
        for (int i = 0; i < sizeof(noiseConfigNames) / sizeof(noiseConfigNames[0]); ++i)
        {
            bool isSelected = (state.noiseConfig == static_cast<NoiseConfiguration>(i));
            if (ImGui::Selectable(noiseConfigNames[i], isSelected))
            {
                state.noiseConfig = static_cast<NoiseConfiguration>(i);
                commands.changeNoiseConfiguration = true;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    
    int octaves = config.octaves;
    if (ImGui::SliderInt("Noise Octaves", &octaves, 1, 8))
    {
        config.octaves = octaves;
        commands.updateHeightMap = true;
    }

    float amplitude = config.amplitude;
    if (ImGui::SliderFloat("Noise Amplitude", &amplitude, 1.0f, 15.0f))
    {
        config.amplitude = amplitude;
        commands.updateHeightMap = true;
    }

    float frequency = config.frequency;
    if (ImGui::SliderFloat("Noise Frequency", &frequency, 0.01f, 0.5f))
    {
        config.frequency = frequency;
        commands.updateHeightMap = true;
    }

    float lacunarity = config.lacunarity;
    if (ImGui::SliderFloat("Noise Lacunarity", &lacunarity, 1.0f, 2.0f))
    {
        config.lacunarity = lacunarity;
        commands.updateHeightMap = true;
    }

    float persistence = config.persistence;
    if (ImGui::SliderFloat("Noise Persistence", &persistence, 0.01f, 0.75f))
    {
        config.persistence = persistence;
        commands.updateHeightMap = true;
    }

    float scale = config.scale;
    if (ImGui::SliderFloat("Noise Scale", &scale, 0.1f, 2.5f))
    {
        config.scale = scale;
        commands.updateHeightMap = true;
    }

    if (ImGui::BeginCombo("Warp Configuration", warpModeNames[static_cast<int>(state.warpMode)]))
    {
        for (int i = 0; i < sizeof(warpModeNames) / sizeof(warpModeNames[0]); ++i)
        {
            bool isSelected = (state.warpMode == static_cast<WarpMode>(i));
            if (ImGui::Selectable(warpModeNames[i], isSelected))
            {
                state.warpMode = static_cast<WarpMode>(i);
                state.warpEnabled = state.warpMode != WarpMode::None;
                commands.changeWarpMode = true;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::BeginDisabled(!state.warpEnabled);
    float warpMultiplier = config.warpMultiplier;
    if (ImGui::SliderFloat("Warp Multiplier", &warpMultiplier, 0.0f, 200.0f))
    {
        config.warpMultiplier = warpMultiplier;
        commands.updateHeightMap = true;
    }

    float warpFrequency = config.warpFrequency;
    if (ImGui::SliderFloat("Warp Frequency", &warpFrequency, 0.000f, 0.03f))
    {
        config.warpFrequency = warpFrequency;
        commands.updateHeightMap = true;
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled();
    ImGui::InputInt("Seed", &state.seed);
    ImGui::EndDisabled();
    if (ImGui::Button("Generate"))
    {
        state.seed = generateSeed();
        commands.changeSeed = true;
    }
}

int NoisePanel::generateSeed()
{
    std::random_device rngDevice;
    std::mt19937 gen(rngDevice());
    std::uniform_int_distribution<int> range(100000000, 999999999); //Seed is 9 Numbers

    return range(gen);
}