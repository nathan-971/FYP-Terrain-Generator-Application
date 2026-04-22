#include "scene/scene.h"

Scene::Scene(
    std::unique_ptr<ITerrainSystem> terrainSystem,
    std::unique_ptr<ISkyboxSystem> skyboxSystem,
    std::unique_ptr<ILightingSystem> lightingSystem
) :
    skybox(std::move(skyboxSystem)),
    terrain(std::move(terrainSystem)),
    lighting(std::move(lightingSystem)),
    flags(0)
{ 
    terrain->Initalize();
    skybox->LoadTextures();
}

Scene::~Scene() { }

void Scene::Update(float deltaTime)
{
    if (flags & static_cast<uint8_t>(UpdateSceneFlag::TerrainMesh))
    {
        terrain->RebuildMesh();
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::RebuildTerrainGenerator))
    {
        terrain->RebuildGenerator();
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::HeightMap))
    {
        terrain->RebuildHeightMap();
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::StartErosion))
    {
        terrain->StartErosion();
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::StopErosion))
    {
        terrain->StopErosion();
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::ResetErosion))
    {
        terrain->ResetErosion();
    }

    if (flags & static_cast<uint8_t>(UpdateSceneFlag::ChangeSkybox))
    {
        skybox->Change();
    }

    terrain->Update(deltaTime);
    flags = 0;
}

void Scene::FlagForUpdate(UpdateSceneFlag flag)
{
    flags |= static_cast<uint8_t>(flag);
}

ITerrainSystem& Scene::Terrain()
{
    return *terrain;
}

const ITerrainSystem& Scene::Terrain() const
{
    return *terrain;
}

ISkyboxSystem& Scene::Skybox()
{
    return *skybox;
}

const ISkyboxSystem& Scene::Skybox() const
{
    return *skybox;
}

ILightingSystem& Scene::Lighting()
{
    return *lighting;
}

const ILightingSystem& Scene::Lighting() const
{
    return *lighting;
}