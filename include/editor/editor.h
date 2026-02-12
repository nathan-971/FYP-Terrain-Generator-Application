#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <random>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "IconsFontAwesome6.h"

#include "core/camera.h"

#include "renderer/scene.h"

class Editor
{
public:
	Editor(Scene& scene, Camera& camera);

	void Update();
	void Render();

private:
	void drawMeshPanel(TerrainConfig& config);
	void drawNoisePanel(TerrainConfig& config);
	void drawLightingPanel();
	void drawErosionPanel(TerrainConfig& config);
	void drawSkyboxPanel();
	void drawExportPanel();
	int generateSeed();

	Scene& scene;
	Camera& camera;

	int seed;
	NoiseConfiguration noiseConfig;
	WarpMode warpMode;
};

#endif