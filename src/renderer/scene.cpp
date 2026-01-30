#include "renderer/scene.h"
#include "exporter/FBXexporter.h"
#include <iostream>

Scene::Scene() :
    generated(false),
    lightPos(glm::vec3(0.0f)),
    lightColor(glm::vec3(0.0f)),
    ambientStrength(0.0f), 
    specularStrength(0.0f),
    shininess(0),
    shadowMap(0),
    shadowMapFBO(0),
    shadowMapHeight(2048),
    shadowMapWidth(2048),
    orthgonalProjection(glm::ortho(-75.0f, 75.0f, -75.0f, 75.0f, 10.0f, 500.0f)),
    terrainShader(),
    depthShader(),
    skyboxShader(),
    terrainMesh(),
    skyboxMesh(),
    skybox(),
    terrainGenerator(config)
{
    //DEFAULT CONFIG VALUES
    config.width = 100;
    config.depth = 100;
    config.resolution = 1.0f;
    config.octaves = 4;
    config.amplitude = 17.0;
    config.frequency = 0.04f;
    config.lacunarity = 1.5f;
    config.persistence = 0.6f;
    config.scale = 1.0f;
    config.warpMultiplier = 0.0f;
}

Scene::~Scene() { }

void Scene::Generate()
{
    if (generated)
    {
        return;
    }

    terrainShader.Load("assets/shaders/vertex.glsl","assets/shaders/fragment.glsl");
    depthShader.Load("assets/shaders/depthVertex.glsl", "assets/shaders/depthFragment.glsl");
    skyboxShader.Load("assets/shaders/skyboxVertex.glsl", "assets/shaders/skyboxFragment.glsl");

    terrainMesh.Create(config.width, config.depth, config.resolution);

    //TEMP LIGHT VALUES
    lightPos = glm::vec3(100.0f);
    lightColor = glm::vec3(1.0f);
    ambientStrength = 0.1f;
    specularStrength = 0.5f;
    shininess = 32;

    terrainShader.Activate();
    terrainShader.setUniformVec3("lightPos", lightPos);
    terrainShader.setUniformVec3("lightColor", lightColor);
    terrainShader.setUniformFloat("ambientStrength", ambientStrength);
    terrainShader.setUniformFloat("specularStrength", specularStrength);
    terrainShader.setUniformInt("shininess", shininess);

    terrainGenerator.setMesh(terrainMesh);
    terrainGenerator.Apply();

    skybox.LoadTextures();

    generateShadowMap();
    generated = true;
}

void Scene::Update()
{
    if (flags == 0)
    {
        return;
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::Mesh))
    {
        terrainMesh.Create(config.width, config.depth, config.resolution);
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::HeightMap))
    {
        terrainGenerator.Apply();
    }

    flags = 0;
}

void Scene::FlagForUpdate(UpdateSceneFlag flag)
{
    flags |= static_cast<uint8_t>(flag);
}

void Scene::Render(Window& window, Camera& camera)
{
    glm::mat4 lightView = glm::lookAt(lightPos, WORLD_ORIGIN, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = orthgonalProjection * lightView;

    renderDepthPass(lightSpaceMatrix);

    window.updateViewport(window.getWidth(), window.getHeight());
    camera.onResize(window.getWidth(), window.getHeight());

    renderScenePass(lightSpaceMatrix, camera);
    if (!skybox.isDisabled())
    {
        renderSkyboxPass(camera);
    }

    camera.updateCameraMatrix(75.0f, 0.05f, 250.0f);
}

void Scene::renderDepthPass(glm::mat4& lightSpaceMatrix)
{
    glViewport(0, 0, shadowMapWidth, shadowMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    depthShader.Activate();

    glm::mat4 model = glm::mat4(1.0f);
    depthShader.setUniformMat("model", model);

    model = glm::translate(model, glm::vec3(0.0f));
    depthShader.setUniformMat("model", model);
    terrainMesh.Draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::renderScenePass(glm::mat4& lightSpaceMatrix, Camera& camera)
{
    terrainShader.Activate();
    terrainShader.setUniformMat("lightSpaceMatrix", lightSpaceMatrix);
    terrainShader.setUniformMat("view", camera.view);
    terrainShader.setUniformMat("projection", camera.projection);
    terrainShader.setUniformVec3("viewPos", camera.Position);

    terrainShader.setUniformVec3("lightPos", lightPos);
    terrainShader.setUniformVec3("lightColor", lightColor);
    terrainShader.setUniformFloat("ambientStrength", ambientStrength);
    terrainShader.setUniformFloat("specularStrength", specularStrength);
    terrainShader.setUniformInt("shininess", shininess);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    terrainShader.setUniformInt("shadowMap", 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f));
    terrainShader.setUniformMat("model", model);
    terrainMesh.Draw();
}

void Scene::renderSkyboxPass(Camera& camera)
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    skyboxShader.Activate();

    glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up);
    view = glm::mat4(glm::mat3(view));

    glm::mat4 projection = camera.projection;

    skyboxShader.setUniformMat("view", view);
    skyboxShader.setUniformMat("projection", camera.projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getActiveTextureId());

    glDisable(GL_CULL_FACE);
    skyboxMesh.Draw();
    glEnable(GL_CULL_FACE);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Scene::generateShadowMap()
{
    glGenFramebuffers(1, &shadowMapFBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    int swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Scene::isGenerated() const
{
    return this->generated;
}

bool Scene::ChangeSkybox(SkyboxOption option) 
{
    if (skybox.Change(option))
    {
        return true;
    }
    return false;
}

TerrainConfig& Scene::getTerrainConfig()
{
    return config;
}

TerrainGenerator& Scene::getTerrainGenerator()
{
    return terrainGenerator;
}

//TEMPORARY FUNCTION
void Scene::exportTerrain()
{
    Exporter* exporter = new FBXExporter();
    try
    {
        if (exporter->Export(terrainMesh, std::string("C:/FBX-OUTPUT/terrain.fbx")))
        {
            std::cout << "saved FBX model file!";
            return;
        }
        std::cout << "Failed to save FBX Model File!";
    }
    catch(const std::exception e)
    {
        std::cout << "Error Exporting Terrain to FBX File!";
        delete exporter;
        return;
    }
    delete exporter;
}