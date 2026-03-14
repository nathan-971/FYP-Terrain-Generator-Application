#include "scene/scene.h"

#include "terrain/noise/noiseconfiguration.h"
#include "terrain/warp/warpmode.h"

#include "utils/terraingeneratorfactory.h"
#include "utils/exporterFactory.h"

#include <iostream>

Scene::Scene(
    std::unique_ptr<ITerrainSystem> terrainSystem,
    std::unique_ptr<ISkyboxSystem> skyboxSystem,
    std::unique_ptr<IExporterFactory> exporterFactory
) :
    skybox(std::move(skyboxSystem)),
    terrain(std::move(terrainSystem)),
    exporterFactory(std::move(exporterFactory)),
    generated(false),
    light(),
    flags(0) 
{ 
    config.width = 100;
    config.depth = 100;
    config.resolution = 0.5f;
    config.rotationSpeed = 10.0f;

    config.octaves = 5;
    config.amplitude = 0.0f;
    config.frequency = 0.0f;
    config.lacunarity = 0.0f;
    config.persistence = 0.0f;
    config.scale = 0.0f;
    config.warpMultiplier = 0.0f;
    config.warpFrequency = 0.0f;

    config.warpMode = WarpMode::None;
    config.noiseConfig = NoiseConfiguration::BaseNoise;

    config.erosionEnabled = false;
}

Scene::~Scene() { }

void Scene::Generate()
{
    if (generated)
    {
        return;
    }

    terrain->Initalize(config);

    skybox->LoadTextures();

    light.position = glm::vec3(-150.0f, 150.0f, -150.0f);
    light.color = glm::vec3(1.0f);
    light.ambient = 0.1f;
    light.specular = 0.1f;
    light.shininess = 15;

    generated = true;
}

void Scene::Update(float deltaTime)
{
    if (flags & static_cast<uint8_t>(UpdateSceneFlag::TerrainMesh))
    {
        terrain->RebuildMesh(config.width, config.depth, config.resolution);
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::RebuildTerrainGenerator))
    {
        terrain->RebuildGenerator(config);
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::HeightMap))
    {
        terrain->RebuildHeightMap(config);
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::ChangeSkybox))
    {
        skybox->Change(config.skyboxOption);
    }
    terrain->Update(config, deltaTime);
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
        glm::vec3(terrain->getMeshTransform().getMatrix()[3]),
        glm::vec3(0, 1, 0)
    );

    frame.lightSpaceMatrix = lightProjection * lightView;
    frame.lightPosition = light.position;
    frame.lightColor = light.color;
    frame.ambientStrength = light.ambient;
    frame.specularStrength = light.specular;
    frame.shininess = light.shininess;

    frame.terrain.modelMatrix = terrain->getMeshTransform().getMatrix();
    frame.terrain.terrainMesh = &terrain->getMesh();

	frame.skybox.skyboxMesh = &skybox->getMesh();
    frame.skybox.enabled = !skybox->isDisabled();
    frame.skybox.skyboxTexture = skybox->getActiveTexture();

    return frame;
}

void Scene::positionAndOrientateCamera(ICamera& camera)
{
    glm::vec3 meshLocalPos = terrain->getMeshTransform().getMatrix()[3];
    camera.setPosition(glm::vec3(
        meshLocalPos.x - 75.0f,
        100.0f,
        meshLocalPos.z - 75.0f
    ));
    camera.setOrientation(glm::normalize(meshLocalPos - camera.getPosition()));
}

void Scene::ExportTerrain(const FileType& type, const std::string& path)
{
    try
    {
        std::unique_ptr<IExporter> exporter = exporterFactory->Create(type);
        if (exporter->Export(terrain->getMesh(), path))
        {
            std::cout << "saved model file!";
            return;
        }
        std::cout << "Failed to save Model File!";
    }
    catch (const std::exception& e)
    {
        std::cout << "Error Exporting Terrain to File!";
        return;
    }
}

Light& Scene::getLight()
{
    return light;
}

TerrainConfig& Scene::getConfig()
{
    return config;
}

void Scene::StartErosion()
{
    terrain->StartErosion();
}

void Scene::StopErosion()
{
    terrain->StopErosion();
}

void Scene::ResetErosion()
{
    terrain->ResetErosion();
}