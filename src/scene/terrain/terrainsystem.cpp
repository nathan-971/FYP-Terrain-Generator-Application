#include "scene/terrain/terrainsystem.h"

#include "scene/mesh/transform.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

TerrainSystem::TerrainSystem(std::unique_ptr<ITerrainGeneratorFactory> generatorFactory)
	: generatorFactory(std::move(generatorFactory)), mesh()
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

TerrainSystem::~TerrainSystem() { }

void TerrainSystem::Initalize()
{
	mesh.Create(config.width, config.depth, config.resolution);
	generator = generatorFactory->Create(config);
	generator->Generate(mesh);
}

void TerrainSystem::Update(float deltaTime)
{
	generator->Update(mesh);

	Transform& meshTransform = mesh.getTransform();
	glm::quat delta = glm::angleAxis(
		glm::radians(config.rotationSpeed) * deltaTime,
		glm::vec3(0, 1, 0)
	);
	meshTransform.rotation = glm::normalize(delta * meshTransform.rotation);
}

void TerrainSystem::RebuildGenerator()
{
	generator = generatorFactory->Create(config);
}

void TerrainSystem::RebuildMesh()
{
	mesh.Create(config.width, config.depth, config.resolution);
}

void TerrainSystem::RebuildHeightMap()
{
	generator->UpdateParameters(config);
	generator->Generate(mesh);
}

void TerrainSystem::StartErosion()
{
	generator->StartErosion();
}

void TerrainSystem::StopErosion()
{
	generator->StopErosion();
}

void TerrainSystem::ResetErosion()
{
	generator->ResetErosion();
}

TerrainConfig& TerrainSystem::getConfig()
{
	return config;
}

glm::vec3 TerrainSystem::getTerrainPosition() const
{
	return glm::vec3(mesh.getTransform().getMatrix()[3]);
}

const TerrainMesh& TerrainSystem::getMesh() const
{
	return mesh;
}

const Transform& TerrainSystem::getMeshTransform() const
{
	return mesh.getTransform();
}