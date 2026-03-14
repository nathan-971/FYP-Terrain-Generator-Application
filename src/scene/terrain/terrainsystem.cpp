#include "scene/terrain/terrainsystem.h"

#include "scene/mesh/transform.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

TerrainSystem::TerrainSystem(std::unique_ptr<ITerrainGeneratorFactory> generatorFactory)
	: generatorFactory(std::move(generatorFactory)), mesh()
{ }

TerrainSystem::~TerrainSystem() { }

void TerrainSystem::Initalize(const TerrainConfig& config)
{
	mesh.Create(config.width, config.depth, config.resolution);
	generator = generatorFactory->Create(config);
	generator->Generate(mesh);
}

void TerrainSystem::Update(const TerrainConfig& config, float deltaTime)
{
	generator->Update(mesh);

	Transform& meshTransform = mesh.getTransform();
	glm::quat delta = glm::angleAxis(
		glm::radians(config.rotationSpeed) * deltaTime,
		glm::vec3(0, 1, 0)
	);
	meshTransform.rotation = glm::normalize(delta * meshTransform.rotation);
}

void TerrainSystem::RebuildGenerator(const TerrainConfig& config)
{
	generator = generatorFactory->Create(config);
}

void TerrainSystem::RebuildMesh(int width, int depth, float resolution)
{
	mesh.Create(width, depth, resolution);
}

void TerrainSystem::RebuildHeightMap(const TerrainConfig& config)
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

TerrainMesh& TerrainSystem::getMesh()
{
	return mesh;
}

Transform& TerrainSystem::getMeshTransform()
{
	return mesh.getTransform();
}