#include "renderer/scene.h"
#include <iostream>
#include <glm/gtx/quaternion.hpp>

Scene::Scene() :
    generated(false),
    terrainMesh(),
    skyboxMesh(),
    skybox(),
    light(),
    terrainGenerator(config),
    terrainTransform(terrainMesh.getTransform()),
    flags(0)
{
    //DEFAULT CONFIG VALUES
    config.width = 100;
    config.depth = 100;
    config.resolution = 1.0f;
    config.octaves = 5;
    config.amplitude = 0.0f;
    config.frequency = 0.0f;
    config.lacunarity = 0.0f;
    config.persistence = 0.0f;
    config.scale = 0.0f;
    config.warpMultiplier = 0.0f;
    config.warpFrequency = 0.0f;
    config.enableErosion = false;
    config.rotationSpeed = 0.25f;
}

Scene::~Scene() { }

void Scene::Generate()
{
    if (generated)
    {
        return;
    }

    terrainMesh.Create(config.width, config.depth, config.resolution);

    terrainGenerator.setMesh(terrainMesh);
    terrainGenerator.Apply();

    skybox.LoadTextures();

    light.position = glm::vec3(100.0f);
    light.color = glm::vec3(1.0f);
    light.ambient = 0.1f;
    light.specular = 0.1f;
    light.shininess = 15;

    generated = true;
}

void Scene::Update()
{
    if (flags & static_cast<uint8_t>(UpdateSceneFlag::Mesh))
    {
        terrainMesh.Create(config.width, config.depth, config.resolution);
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::HeightMap))
    {
        terrainGenerator.Apply();
    }

    std::cout << "Delta Time: " << Time::deltaTime << std::endl;

    terrainTransform.rotation = glm::rotate(
        terrainTransform.rotation,
        Time::deltaTime * config.rotationSpeed,
        glm::vec3(0, 1, 0)
    );
    flags = 0;
    std::cout << "TRANSFORMATION: "
        << terrainTransform.rotation.x << " "
        << terrainTransform.rotation.y << " "
        << terrainTransform.rotation.z << "\n";
}

void Scene::FlagForUpdate(UpdateSceneFlag flag)
{
    flags |= static_cast<uint8_t>(flag);
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

FrameData Scene::getFrameData(Camera& camera)
{
    FrameData frame;
    frame.viewMatrix = camera.view;
    frame.projectionMatrix = camera.projection;
    frame.cameraPosition = camera.position;
    frame.cameraOrientation = camera.orientation;
    frame.cameraUp = camera.up;

    glm::mat4 lightView = glm::lookAt(light.position, glm::vec3(0.0f), glm::vec3(0, 1, 0));
    frame.lightSpaceMatrix = lightView;

    frame.lightPosition = light.position;
    frame.lightColor = light.color;

    frame.ambientStrength = light.ambient;
    frame.specularStrength = light.specular;
    frame.shininess = light.shininess;

    return frame;
}

void Scene::ExportTerrain(FileType type)
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
    catch (const std::exception e)
    {
        std::cout << "Error Exporting Terrain to FBX File!";
        delete exporter;
        return;
    }
    delete exporter;
}

TerrainConfig& Scene::getTerrainConfig()
{
    return config;
}

TerrainGenerator& Scene::getTerrainGenerator()
{
    return terrainGenerator;
}

TerrainMesh& Scene::getTerrainMesh()
{
    return terrainMesh;
}

Transform& Scene::getTerrainTransform()
{
    return terrainTransform;
}

Skybox& Scene::getSkybox()
{
    return skybox;
}

SkyboxMesh& Scene::getSkyboxMesh()
{
    return skyboxMesh;
}

Light& Scene::getLight()
{
    return light;
}