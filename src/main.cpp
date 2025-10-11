#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "renderer/shader.h"
#include "core/camera.h"
#include "renderer/mesh.h"
#include "terrain/terraingenerator.h"

#define GL_VERSION_NUM 3

#define VSYNC 1
#define NOVSYNC 0

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

#define WORLD_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void enableWireFrame(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_NUM);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_NUM);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FYP-Prototype", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(VSYNC);

    Camera camera(SCR_WIDTH, SCR_HEIGHT, WORLD_ORIGIN);
    glfwSetWindowUserPointer(window, &camera);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //ImGui Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    double previousTime = glfwGetTime();
    int frames = 0;
    static int fps = 0;

    Shader shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

    TerrainConfig config;
	config.width = 32;
	config.depth = 32;
    config.resolution = 1.0f;
    config.octaves = 4;
    config.amplitude = 2.5f;
    config.frequency = 0.01f;
	config.lacunarity = 2.0f;
	config.persistence = 0.25f;

    Mesh mesh(config.width, config.depth, config.resolution, shader.progID);
    TerrainGenerator tg(config);

    tg.setMesh(mesh);
    tg.Apply();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        enableWireFrame(window);

        //ImGui Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Debug Window
        ImGui::Begin("Debug Information");

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;
        camera.deltaTime = deltaTime;

        frames++;
        if (currentFrame - previousTime >= 1.0)
        {
            fps = frames;
            frames = 0;
            previousTime = currentFrame;
        }
        ImGui::Text("FPS: %d", fps);
        ImGui::Text("Camera Position: ( %.2f, %.2f, %.2f )", camera.Position.x, camera.Position.y, camera.Position.z);
        ImGui::Text("Camera Speed: %.2f", camera.speed);

        if(ImGui::SliderInt("Terrain Width", (int*)&config.width, TERRAIN_MIN_WIDTH, TERRAIN_MAX_DEPTH))
        {
            mesh.reMesh(config.width, config.depth, config.resolution);
            tg.Apply();
		}

        if (ImGui::SliderInt("Terrain Depth", (int*)&config.depth, TERRAIN_MIN_DEPTH, TERRAIN_MAX_DEPTH))
        {
            mesh.reMesh(config.width, config.depth, config.resolution);
            tg.Apply();
        }

        if (ImGui::SliderFloat("Terrain Resolution", (float*)&config.resolution, 1.0f, 0.5f))
        {
            mesh.reMesh(config.width, config.depth, config.resolution);
            tg.Apply();
        }

        if (ImGui::SliderInt("Noise Octaves", (int*)&config.octaves, 1, 8))
        {
			tg.getConfig().octaves = config.octaves;
            tg.Apply();
        }

        if (ImGui::SliderFloat("Noise Amplitude", (float*)&config.amplitude, 1.0f, 20.0f))
        {
			tg.getConfig().amplitude = config.amplitude;
            tg.Apply();
        }

        if (ImGui::SliderFloat("Noise Frequency", (float*)&config.frequency, 0.01f, 0.1f))
        {
            tg.getConfig().frequency = config.frequency;
            tg.Apply();
        }

        if (ImGui::SliderFloat("Noise Lacunarity", (float*)&config.lacunarity, 1.0f, 10.0f))
        {
            tg.getConfig().lacunarity = config.lacunarity;
            tg.Apply();
        }

        if (ImGui::SliderFloat("Noise Persistence", (float*)&config.persistence, 0.01f, 5.0f))
        {
            tg.getConfig().persistence = config.persistence;
            tg.Apply();
        }

        ImGui::End();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Activate();
        mesh.Draw(shader.progID);

        camera.updateCameraMatrix(75.0f, 0.05f, 250.0f);
        camera.Inputs(window);
        shader.setUniformMat("camMatrix", camera.cameraMatrix);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    if (camera)
    {
        camera->onResize(width, height);
    }
}

void enableWireFrame(GLFWwindow* window)
{
    static bool enabled = false;
    static bool lastFramePressed = false;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
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