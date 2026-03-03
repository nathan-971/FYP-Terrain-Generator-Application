#include "renderer/scene.h"
#include "exporter/FBXexporter.h"
#include "exporter/OBJexporter.h"
#include <iostream>

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
    config.resolution = 0.5f;
    config.octaves = 5;
    config.amplitude = 0.0f;
    config.frequency = 0.0f;
    config.lacunarity = 0.0f;
    config.persistence = 0.0f;
    config.scale = 0.0f;
    config.warpMultiplier = 0.0f;
    config.warpFrequency = 0.0f;
    config.rotationSpeed = 25.0f;
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

    light.position = glm::vec3(-150.0f, 150.0f, -150.0f);
    light.color = glm::vec3(1.0f);
    light.ambient = 0.1f;
    light.specular = 0.1f;
    light.shininess = 15;

    generated = true;
}

void Scene::Update(float deltaTime)
{
    if (flags & static_cast<uint8_t>(UpdateSceneFlag::Mesh))
    {
        terrainMesh.Create(config.width, config.depth, config.resolution);
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::HeightMap))
    {
        terrainGenerator.Apply();
    }

    glm::quat delta = glm::angleAxis(
        glm::radians(config.rotationSpeed) * deltaTime,
        glm::vec3(0, 1, 0)
    );
    terrainTransform.rotation = glm::normalize(delta * terrainTransform.rotation);
    flags = 0;
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
	return skybox.Change(option);
}

FrameData Scene::getFrameData(const ICamera& camera)
{
    FrameData frame;

    frame.viewMatrix = camera.getView();
    frame.projectionMatrix = camera.getProjection();
    frame.cameraPosition = camera.getPosition();

    glm::mat4 lightProjection = glm::ortho(
        -80.0f, 80.0f,
        -80.0f, 80.0f,
        1.0f, 120.0f 
    );

    glm::mat4 lightView = glm::lookAt(
        light.position,
        glm::vec3(terrainTransform.getMatrix()[3]),
        glm::vec3(0, 1, 0)
    );

    frame.lightSpaceMatrix = lightProjection * lightView;
    frame.lightPosition = light.position;
    frame.lightColor = light.color;
    frame.ambientStrength = light.ambient;
    frame.specularStrength = light.specular;
    frame.shininess = light.shininess;

    frame.terrain.modelMatrix = terrainTransform.getMatrix();
    frame.terrain.terrainMesh = &terrainMesh;

	frame.skybox.skyboxMesh = &skyboxMesh;
    frame.skybox.enabled = !skybox.isDisabled();
    frame.skybox.skyboxTexture = skybox.getActiveTextureId();

    return frame;
}

void Scene::positionAndOrientateCamera(ICamera& camera)
{
    glm::vec3 meshLocalPos = terrainTransform.getMatrix()[3];
    camera.setPosition(glm::vec3(
        meshLocalPos.x - 75.0f,
        100.0f,
        meshLocalPos.z - 75.0f
    ));
    camera.setOrientation(glm::normalize(meshLocalPos - camera.getPosition()));
}

void Scene::ExportTerrain(const FileType& type, const std::string& path)
{
    Exporter* exporter = nullptr;
    try
    {
        switch (type)
        {
            case FileType::FBX:
            {
                exporter = new FBXExporter();
                break;
            }
            case FileType::OBJ:
            {
                exporter = new OBJExporter();
                break;
            }
            default:
            {
                exporter = new FBXExporter();
                break;
            }
        }
        if (exporter->Export(terrainMesh, path))
        {
            std::cout << "saved model file!";
            return;
        }
        std::cout << "Failed to save  Model File!";
    }
    catch (const std::exception e)
    {
        std::cout << "Error Exporting Terrain to File!";
        delete exporter;
		exporter = nullptr;
        return;
    }
    delete exporter;
    exporter = nullptr;
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

Light& Scene::getLight()
{
    return light;
}