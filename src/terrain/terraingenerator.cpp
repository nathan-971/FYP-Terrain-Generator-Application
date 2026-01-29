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
	
void TerrainGenerator::setMesh(TerrainMesh& mesh)
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

void TerrainGenerator::Apply()
{
	//Temporary Structure, Requires Refactor
	applyNoise();
	applyErosion();
	terrainMesh->recalculateNormals(this->config.width, this->config.depth, this->config.resolution);

	terrainMesh->UpdateBuffers();
}

void TerrainGenerator::applyNoise()
{
	int octaves = this->config.octaves;
	float lacunarity = this->config.lacunarity;
	float persistence = this->config.persistence;
	float scale = this->config.scale;

	for (auto& vertex : terrainMesh->GetVertices())
	{
		float amplitude = this->config.amplitude;
		float frequency = this->config.frequency;
		float totalNoise = 0.0f;

		for (int i = 0; i < octaves; i++)
		{
			totalNoise += noise->Get(vertex.position.x * scale, vertex.position.z * scale, frequency) * amplitude;

			frequency *= lacunarity;
			amplitude *= persistence;
		}

		float vertHeight = totalNoise;
		vertex.position.y = vertHeight;
	}
}

void TerrainGenerator::applyErosion()
{
	
}