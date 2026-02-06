#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "IconsFontAwesome6.h"

#include <random>
#include <iostream>

#include "core/window.h"
#include "core/camera.h"
#include "core/time.h"

#include "renderer/scene.h"
#include "renderer/renderer.h"
#include "exporter/exporter.h"

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

#define WORLD_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

void enableWireFrame(Window& window);
int generateSeed();

int main()
{
    Window window(SCR_HEIGHT, SCR_WIDTH, std::string("Terrain Generator Program"), true);
    Camera camera(SCR_WIDTH, SCR_HEIGHT, WORLD_ORIGIN);
    Scene scene;
    Renderer renderer;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    //ImGui Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontDefault();

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphOffset.y = +1.0f;
    config.GlyphOffset.x = +1.0f;

    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF("assets/fonts/fa-solid-900.ttf", 15.0f, &config, icon_ranges);

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    ImGui_ImplGlfw_InitForOpenGL(window.getNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImVec2 buttonSize(30, 30);


    Time::applicationTime = window.getTime();
    float lastFrameTime = Time::applicationTime;

    float fpsTimer = 0.0f;
    int frames = 0;
    static int fps = 0;

    const char* noiseConfigNames[] = { "Base Noise", "Ridged Noise" };
    static NoiseConfiguration selectedConfig = NoiseConfiguration::BaseNoise;

    const char* warpModeNames[] = { "None", "Single", "Double" };
    static WarpMode selectedWarpMode = WarpMode::None;

    static bool disableControls = false;

    int seed = generateSeed();

    while (!window.shouldClose())
    {
        window.pollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        TerrainConfig& config = scene.getTerrainConfig();

        //ImGui Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!scene.isGenerated())
        {
            ImGui::Begin("Generate Terrain");
            ImGui::SeparatorText("Input Terrain Configuration");

            ImGui::SliderInt("Width", (int*)&config.width, TERRAIN_MIN_WIDTH, TERRAIN_MAX_WIDTH);
            ImGui::SliderInt("Depth", (int*)&config.depth, TERRAIN_MIN_DEPTH, TERRAIN_MAX_DEPTH);
            ImGui::SliderFloat("Resolution", &config.resolution, 0.5f, 2.0f);
            ImGui::SliderInt("Octaves", &config.octaves, 1, 8);
            ImGui::SliderFloat("Amplitude", &config.amplitude, 1.0f, 20.0f);
            ImGui::SliderFloat("Frequency", &config.frequency, 0.01f, 0.1f);
            ImGui::SliderFloat("Lacunarity", &config.lacunarity, 1.0f, 2.0f);
            ImGui::SliderFloat("Persistence", &config.persistence, 0.01f, 1.0f);

            if (ImGui::Button("Generate Terrain"))
            {
                scene.Generate();
            }
            ImGui::ShowDemoWindow();
            ImGui::End();
        }
        else
        {
            enableWireFrame(window);
            ImGui::Begin("Terrain Settings");

            float currentFrame = window.getTime();
            Time::applicationTime = currentFrame;

            Time::deltaTime = currentFrame - lastFrameTime;
            lastFrameTime = currentFrame;

            fpsTimer += Time::deltaTime;
            frames++;

            if (fpsTimer >= 1.0f)
            {
                fps = frames;
                frames = 0;
                fpsTimer = 0.0f;
            }

            ImGui::Text("FPS: %d", fps);
            ImGui::Text("Camera Position: ( %.2f, %.2f, %.2f )", camera.position.x, camera.position.y, camera.position.z);
            ImGui::Text("Camera Speed: %.2f", camera.speed);
#pragma region IMGUI CONTROLS
#pragma region MESH CONTROLS
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
#pragma endregion
#pragma region NOISE CONTROLS
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
#pragma endregion
#pragma region EROSION CONTROLS
            ImGui::SeparatorText("Erosion Settings");
            if (ImGui::Checkbox("Enable Erosion", &config.enableErosion))
            {
                scene.getTerrainGenerator().toggleErosion();
                scene.FlagForUpdate(UpdateSceneFlag::HeightMap);
            }
#pragma endregion
#pragma region LIGHT CONTROLS
            ImGui::SeparatorText("Lighting Configuration");
            ImGui::SliderFloat3("Light Position", &scene.getLight().position.x, 0.0f, 100.0f);
            ImGui::SliderFloat3("Light Color", &scene.getLight().color.x, 0.0f, 1.0f);
            ImGui::SliderFloat("Ambient Strength", &scene.getLight().ambient, 0.0f, 1.0f);
            ImGui::SliderInt("Shininess", &scene.getLight().shininess, 2, 256);
            ImGui::SliderFloat("Specular Strength", &scene.getLight().specular, 0.0f, 1.0f);
#pragma endregion
#pragma region SKYBOX CONTROLS
            ImGui::SeparatorText("Skybox Settings");
            if (ImGui::Button(ICON_FA_CLOUD_SUN, buttonSize))
            {
                scene.ChangeSkybox(SkyboxOption::MORNING);
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_SUN, buttonSize))
            {
                scene.ChangeSkybox(SkyboxOption::NOON);
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_MOON, buttonSize))
            {
                scene.ChangeSkybox(SkyboxOption::NIGHT);
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_CIRCLE_XMARK, buttonSize))
            {
                scene.ChangeSkybox(SkyboxOption::NONE);
            }
#pragma endregion
#pragma region EXPORT CONTROLS
            ImGui::SeparatorText("Export Terrain");
            if (ImGui::Button(ICON_FA_FLOPPY_DISK, buttonSize))
            {
                scene.ExportTerrain(FileType::FBX);
            }
            ImGui::SameLine();
            ImGui::Text("Save Terrain");
#pragma endregion

            ImGui::End();

            camera.Inputs(window.getNativeWindow());
            camera.updateCameraMatrix(75.0f, 0.05f, 250.0f);

            scene.Update();
            renderer.RenderScene(window, camera, scene);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void enableWireFrame(Window& window)
{
    static bool enabled = false;
    static bool lastFramePressed = false;

    if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_M) == GLFW_PRESS)
    {
        if (!lastFramePressed)
        {
            enabled = !enabled;
            glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
            std::cout << "Toggling Wireframe Mode: " << (enabled ? "Enabled" : "Disabled") << std::endl;
        }
        lastFramePressed = true;
    }
    else
    {
        lastFramePressed = false;
    }
}

int generateSeed()
{
    std::random_device rngDevice;
    std::mt19937 gen(rngDevice());
    std::uniform_int_distribution<int> range(100000000, 999999999); //Seed is 9 Numbers

    return range(gen);
}