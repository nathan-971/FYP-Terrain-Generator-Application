#include "terrain/terraingenerator.h"
#include <iostream>

TerrainGenerator::TerrainGenerator(TerrainConfig& config)
	: config(config), noise(nullptr), terrainMesh(nullptr), shaderProgram(0), noiseConfig(NoiseConfiguration::BaseNoise), warpMode(WarpMode::None)
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

void TerrainGenerator::setNoiseConfiguration(NoiseConfiguration& noiseConfig)
{
	this->noiseConfig = noiseConfig;
}

void TerrainGenerator::setWarpMode(WarpMode& warpMode)
{
	this->warpMode = warpMode;
}

void TerrainGenerator::Apply()
{
	switch (noiseConfig)
	{
		case NoiseConfiguration::BaseNoise:
		{
			applyBaseNoise();
			break;
		}
		case NoiseConfiguration::RidgedNoise:
		{
			applyRidgedNoise();
			break;
		}
		default:
		{
			applyBaseNoise();
			break;
		}
	}
	applyErosion();

	terrainMesh->recalculateNormals(this->config.width, this->config.depth, this->config.resolution);
	terrainMesh->UpdateBuffers();
}

void TerrainGenerator::applyRidgedNoise()
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
		float prev = 1.0f;

		for (int i = 0; i < octaves; i++)
		{
			float n = noise->Get(vertex.position.x * scale * frequency, vertex.position.z * scale * frequency);
			n = 1.0f - std::abs(n);
			n *= n;

			totalNoise += n * amplitude * prev;
			prev = n;

			frequency *= lacunarity;
			amplitude *= persistence;
		}

		float vertHeight = totalNoise;
		vertex.position.y = vertHeight;
	}
}

void TerrainGenerator::applyBaseNoise()
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
			float n = noise->Get(vertex.position.x * scale * frequency, vertex.position.z * scale * frequency);
			totalNoise += n * amplitude;

			frequency *= lacunarity;
			amplitude *= persistence;
		}

		float vertHeight = totalNoise;
		vertex.position.y = vertHeight;
	}
}

void TerrainGenerator::applyErosion()
{
	//int vertCountX = static_cast<unsigned int>(config.width / config.resolution) + 1;
	//int vertCountZ = static_cast<unsigned int>(config.depth / config.resolution) + 1;
	//std::vector<float> heightMap;
	//
	//for (Vertex& v : terrainMesh->GetVertices())
	//{
	//	heightMap.push_back(v.position.y);
	//}
	//
	//int erosionIterations = 50;
	//float talus = 1.0f;
	//float erosionStrength = 0.5f;

	//for (int i = 0; i < erosionIterations; i++)
	//{
	//	std::vector<float> erosionMap(heightMap.size(), 0.0f);

	//	for (int z = 0; z < vertCountZ - 1; z++)
	//	{
	//		for (int x = 0; x < vertCountX - 1; x++)
	//		{
	//			int index = z * vertCountX + x;
	//			float currentHeight = heightMap[index];

	//		}
	//	}
	//}
}