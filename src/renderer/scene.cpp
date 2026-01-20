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
    terrainShader{ },
    depthShader{ },
    terrainMesh{ },
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

    camera.updateCameraMatrix(75.0f, 0.05f, 250.0f);
}

void Scene::renderDepthPass(glm::mat4& lightSpaceMatrix)
{
    glViewport(0, 0, shadowMapWidth, shadowMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    depthShader.Activate();
    depthShader.setUniformMat("lightSpaceMatrix", lightSpaceMatrix);
    depthShader.setUniformMat("model", glm::mat4(1.0f));

    terrainMesh.Draw(depthShader.progID);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::renderScenePass(glm::mat4& lightSpaceMatrix, Camera& camera)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    terrainShader.Activate();
    terrainShader.setUniformMat("lightSpaceMatrix", lightSpaceMatrix);
    terrainShader.setUniformMat("camMatrix", camera.cameraMatrix);
    terrainShader.setUniformVec3("viewPos", camera.Position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    terrainShader.setUniformInt("shadowMap", 0);

    terrainMesh.Draw(terrainShader.progID);
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

TerrainConfig& Scene::getTerrainConfig()
{
    return config;
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
        std::cout << "ERROR EXPORTING FILE";
        delete exporter;
        return;
    }
    delete exporter;
}