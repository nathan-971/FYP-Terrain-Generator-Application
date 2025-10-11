#include "terrain/terraingenerator.h"

TerrainGenerator::TerrainGenerator(const TerrainConfig& config) 
	: config(config), terrainMesh(nullptr), shaderProgram(0) { }

TerrainGenerator::~TerrainGenerator() { }

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
			totalNoise += PerlinNoise::perlin2D(vertex.position.x, vertex.position.z, frequency) * amplitude;
			
			frequency *= lacunarity; //Increase frequency as layers increase
			amplitude *= persistence; //Decrease amplitude as layers increase
		}

		float vertHeight = totalNoise;
		vertex.position.y = vertHeight;	
		if (vertHeight < 0.05f)
		{
			vertex.color = glm::vec3(0.1f, 0.5f, 1.0f); //Deep Water
		}
		else if (vertHeight < 0.1f)
		{
			vertex.color = glm::vec3(0.2f, 0.7f, 1.0f); //Water
		}
		else if (vertHeight < 0.5f)
		{
			vertex.color = glm::vec3(0.8f, 0.8f, 0.6f); //Sand
		}
		else if (vertHeight < 3.0f)
		{
			vertex.color = glm::vec3(0.1f, 0.8f, 0.1f); //Grass
		}
		else if (vertHeight < 6.0f)
		{
			vertex.color = glm::vec3(0.5f, 0.5f, 0.5f); //Rock
		}
		else
		{
			vertex.color = glm::vec3(0.8f, 0.8f, 0.8f); //Snow
		}
	}
	terrainMesh->UpdateBuffers();
}