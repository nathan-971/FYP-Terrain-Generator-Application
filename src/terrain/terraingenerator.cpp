#include "terrain/terraingenerator.h"
#include <iostream>

TerrainGenerator::TerrainGenerator(TerrainConfig& config)
	: config(config), noise(nullptr), terrainMesh(nullptr), shaderProgram(0)
{
	noise = new PerlinNoise();
}

TerrainGenerator::~TerrainGenerator() 
{
	delete noise;
	noise = nullptr;
}
	
void TerrainGenerator::setMesh(Mesh& mesh)
{
	terrainMesh = &mesh;
}

void TerrainGenerator::setShaderProgram(unsigned int shaderProgram)
{
	this->shaderProgram = shaderProgram;
}

TerrainConfig& TerrainGenerator::getConfig()
{
	return this->config;
}

void TerrainGenerator::Apply() const
{
	int octaves = this->config.octaves;
	float lacunarity = this->config.lacunarity;
	float persistence = this->config.persistence;

	for(auto& vertex : terrainMesh->GetVertices())
	{
		float amplitude = this->config.amplitude;
		float frequency = this->config.frequency;
		float totalNoise = 0.0f;

		for(int i = 0; i < octaves; i++)
		{
			totalNoise += noise->Get(vertex.position.x, vertex.position.z, frequency) * amplitude;
			
			frequency *= lacunarity;
			amplitude *= persistence;
		}

		float vertHeight = totalNoise;
		vertex.position.y = vertHeight;
	}
	terrainMesh->recalculateNormals(this->config.width, this->config.depth, this->config.resolution);

	for(auto& vertex : terrainMesh->GetVertices())
	{
		float slope = terrainMesh->GetSlopeAt(
			vertex.position.x,
			vertex.position.z,
			this->config.width,
			this->config.depth,
			this->config.resolution
		);

		if (slope < 20.0f)
		{
			vertex.color = glm::vec3(0.1f, 0.6f, 0.1f);
		}
		else if (slope < 35.0f)
		{
			vertex.color = glm::vec3(0.45f, 0.35f, 0.2f);
		}
		else if (slope < 65.0f)
		{
			vertex.color = glm::vec3(0.5f, 0.5f, 0.5f);
		}
		else
		{
			vertex.color = glm::vec3(0.9f, 0.9f, 0.9f);
		}
	}
	terrainMesh->UpdateBuffers();
}