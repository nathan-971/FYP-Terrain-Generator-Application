#include "UI/editor/editor.h"
#include "UI/panels/meshpanel.h"
#include "UI/panels/noisepanel.h"
#include "UI/panels/lightingpanel.h"
#include "UI/panels/erosionpanel.h"
#include "UI/panels/skyboxpanel.h"
#include "UI/panels/exportpanel.h"
#include "UI/panels/heightmappanel.h"
#include "renderer/updatesceneflag.h"

Editor::Editor(Scene& scene) : 
	scene(scene),
    ctx{ scene, scene.getTerrainConfig() }
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
}

void Editor::ApplyCommands()
{
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

	if (ctx.commands.changeWarpMode)
    {
        scene.getTerrainGenerator().setWarpMode(ctx.state.warpMode);
        ctx.commands.changeWarpMode = false;
    }

    if (ctx.commands.changeNoiseConfiguration)
    {
        scene.getTerrainGenerator().setNoiseConfiguration(ctx.state.noiseConfig);
        ctx.commands.changeNoiseConfiguration = false;
	}

    if(ctx.commands.changeErosionEnabled)
    {
        scene.getTerrainGenerator().setErosionEnabled(ctx.state.erosionEnabled);
        ctx.commands.changeErosionEnabled = false;
	}

    if (ctx.commands.changeSkybox)
    {
		scene.ChangeSkybox(ctx.state.skyboxOption);
		ctx.commands.changeSkybox = false;
    }

	if (ctx.commands.exportTerrain)
    {
		scene.ExportTerrain(FileType::FBX); // Default to FBX for now
        ctx.commands.exportTerrain = false;
    }

    if (ctx.commands.newSeed != 0)
    {
        ctx.state.seed = ctx.commands.newSeed;
        scene.getTerrainGenerator().setSeed(ctx.state.seed);
		scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
		ctx.commands.newSeed = 0;
    }
}