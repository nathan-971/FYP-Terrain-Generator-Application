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
    glFrontFace(GL_CW);

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

    Shader terrainShader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
    Shader depthShader("assets/shaders/depthVertex.glsl", "assets/shaders/depthFragment.glsl");

    TerrainConfig config;
    config.width = 100;
    config.depth = 100;
    config.resolution = 1.0f;
    config.octaves = 4;
    config.amplitude = 17.0;
    config.frequency = 0.04f;
    config.lacunarity = 1.5f;
    config.persistence = 0.6f;

    //TEMPORARY LIGHTING VALUES
    glm::vec3 lightPos = glm::vec3(100.0f);
    glm::vec3 lightColor = glm::vec3(1.0f);
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    int shininess = 32;

    //TEMPORARY LIGHTING UNIFORMS
    terrainShader.Activate();
    terrainShader.setUniformVec3("lightPos", lightPos);
    terrainShader.setUniformVec3("lightColor", lightColor);
    terrainShader.setUniformFloat("ambientStrength", ambientStrength);
    terrainShader.setUniformFloat("specularStrength", specularStrength);
    terrainShader.setUniformInt("shininess", shininess);

    Mesh mesh(config.width, config.depth, config.resolution, terrainShader.progID);
    TerrainGenerator tg(config);

    unsigned int shadowMapFrameBufferObj;
    glGenFramebuffers(1, &shadowMapFrameBufferObj);

    unsigned int shadowMapWidth = 2048;
    unsigned int shadowMapHeight = 2048;
    unsigned int shadowMap = 0;
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

    int swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFrameBufferObj);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::mat4 orthgonalProjection = glm::ortho(-75.0f, 75.0f, -75.0f, 75.0f, 10.0f, 500.0f);

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

        if (ImGui::SliderInt("Terrain Width", (int*)&config.width, TERRAIN_MIN_WIDTH, TERRAIN_MAX_WIDTH))
        {
            mesh.createMesh(config.width, config.depth, config.resolution);
            tg.getConfig().width = config.width;
            tg.Apply();
        }

        if (ImGui::SliderInt("Terrain Depth", (int*)&config.depth, TERRAIN_MIN_DEPTH, TERRAIN_MAX_DEPTH))
        {
            mesh.createMesh(config.width, config.depth, config.resolution);
            tg.getConfig().depth = config.depth;
            tg.Apply();
        }

        if (ImGui::SliderFloat("Terrain Resolution", (float*)&config.resolution, 1.0f, 0.5f))
        {
            mesh.createMesh(config.width, config.depth, config.resolution);
            tg.getConfig().resolution = config.resolution;
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

        if (ImGui::SliderFloat("Noise Lacunarity", (float*)&config.lacunarity, 1.0f, 2.0f))
        {
            tg.getConfig().lacunarity = config.lacunarity;
            tg.Apply();
        }

        if (ImGui::SliderFloat("Noise Persistence", (float*)&config.persistence, 0.01f, 1.0f))
        {
            tg.getConfig().persistence = config.persistence;
            tg.Apply();
        }

        if (ImGui::SliderFloat3("Light Position", &lightPos.x, 0.0f, 100.0f))
        {
            terrainShader.setUniformVec3("lightPos", lightPos);
        }

        if (ImGui::SliderFloat3("Light Color", &lightColor.x, 0.0f, 1.0f))
        {
            terrainShader.setUniformVec3("lightColor", lightColor);
        }

        if (ImGui::SliderFloat("Ambient Strength", (float*)&ambientStrength, 0.0f, 1.0f))
        {
            terrainShader.setUniformFloat("ambientStrength", ambientStrength);
        }

        if (ImGui::SliderInt("Shininess", (int*)&shininess, 2, 256))
        {
            terrainShader.setUniformInt("shininess", shininess);
        }

        if (ImGui::SliderFloat("Specular Strength", (float*)&specularStrength, 0.0f, 1.0f))
        {
            terrainShader.setUniformFloat("specularStrength", specularStrength);
        }
        ImGui::Text("Shadow Map Texture:");
        ImGui::Image((int*)shadowMap, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        glm::mat4 lightView = glm::lookAt(lightPos, WORLD_ORIGIN, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = orthgonalProjection * lightView;

        //FIRST PASS - RENDER DEPTH OF SCENE FROM LIGHTS PERSPECTIVE
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFrameBufferObj);
        glClear(GL_DEPTH_BUFFER_BIT);

        depthShader.Activate();
        depthShader.setUniformMat("lightSpaceMatrix", lightSpaceMatrix);
        depthShader.setUniformMat("model", glm::mat4(1.0f));

        mesh.Draw(depthShader.progID);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //SECOND PASS - RENDER NORMAL SCENE USING THE GENERATED DEPTH MAP
		framebuffer_size_callback(window, camera.width, camera.height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        terrainShader.Activate();
        terrainShader.setUniformMat("lightSpaceMatrix", lightSpaceMatrix);
        terrainShader.setUniformMat("camMatrix", camera.cameraMatrix);
        terrainShader.setUniformVec3("viewPos", camera.Position);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        terrainShader.setUniformInt("shadowMap", 0);

        mesh.Draw(terrainShader.progID);

        camera.updateCameraMatrix(75.0f, 0.05f, 250.0f);
        camera.Inputs(window);

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