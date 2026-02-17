#include "UI/panels/noisepanel.h"

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
                commands.updateHeightMap = true;
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
        commands.updateHeightMap = true;
    }

    if (ImGui::SliderFloat("Noise Amplitude", static_cast<float*>(&config.amplitude), 1.0f, 15.0f))
    {
        commands.updateHeightMap = true;
    }

    if (ImGui::SliderFloat("Noise Frequency", static_cast<float*>(&config.frequency), 0.01f, 0.5f))
    {
        commands.updateHeightMap = true;
    }

    if (ImGui::SliderFloat("Noise Lacunarity", static_cast<float*>(&config.lacunarity), 1.0f, 2.0f))
    {
        commands.updateHeightMap = true;
    }

    if (ImGui::SliderFloat("Noise Persistence", static_cast<float*>(&config.persistence), 0.01f, 0.75f))
    {
        commands.updateHeightMap = true;
    }

    if (ImGui::SliderFloat("Noise Scale", static_cast<float*>(&config.scale), 0.1f, 2.5f))
    {
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

                commands.changeWarpMode = true;
				commands.updateHeightMap = true;
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
		commands.updateHeightMap = true;
    }

    if (ImGui::SliderFloat("Wrap Freqeuncy", static_cast<float*>(&config.warpFrequency), 0.000f, 0.03f))
    {
		commands.updateHeightMap = true;
    }

    ImGui::BeginDisabled();
    ImGui::InputInt("Seed", &state.seed);
    ImGui::EndDisabled();
    if (ImGui::Button("Generate"))
    {
        commands.newSeed = generateSeed();
    }
}

int NoisePanel::generateSeed()
{
    std::random_device rngDevice;
    std::mt19937 gen(rngDevice());
    std::uniform_int_distribution<int> range(100000000, 999999999); //Seed is 9 Numbers

    return range(gen);
}