#include "terrain/terraingenerator.h"
#include <iostream>

TerrainGenerator::TerrainGenerator(TerrainConfig& config)
	: config(config), 
	noise(nullptr), 
	terrainMesh(nullptr), 
	shaderProgram(0),
	seed(0),
	noiseConfig(NoiseConfiguration::BaseNoise), 
	warpMode(WarpMode::None)
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
	this->terrainMesh = &mesh;
}

void TerrainGenerator::setShaderProgram(unsigned int shaderProgram)
{
	this->shaderProgram = shaderProgram;
}

void TerrainGenerator::setSeed(int& seed)
{
	this->seed = seed;
}

void TerrainGenerator::setNoiseConfiguration(NoiseConfiguration& noiseConfig)
{
	this->noiseConfig = noiseConfig;
}

void TerrainGenerator::setWarpMode(WarpMode& warpMode)
{
	this->warpMode = warpMode;
}

void TerrainGenerator::toggleErosion()
{
	this->erosionToggled = erosionToggled ? false : true;
}

TerrainConfig& TerrainGenerator::getConfig()
{
	return this->config;
}

void TerrainGenerator::Apply()
{
	noise->ApplySeed(seed);
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

	if (erosionToggled)
	{
		applyErosion();
	}

	terrainMesh->recalculateNormals(this->config.width, this->config.depth, this->config.resolution);
	terrainMesh->UpdateBuffers();
}

void TerrainGenerator::applyRidgedNoise()
{
	int octaves = this->config.octaves;
	float lacunarity = this->config.lacunarity;
	float persistence = this->config.persistence;
	float scale = this->config.scale;

	float warpFrequency = config.warpFrequency;
	float warpAmplitude = config.warpMultiplier;

	for (auto& vertex : terrainMesh->GetVertices()) 
	{
		float amplitude = this->config.amplitude;
		float frequency = this->config.frequency;
		float totalNoise = 0.0f;
		float prev = 1.0f;

		float noiseX = vertex.position.x * scale;
		float noiseZ = vertex.position.z * scale;

		switch (warpMode)
		{
			case WarpMode::Single:
			{
				warpSingle(vertex, noiseX, noiseZ);
				break;
			}
			case WarpMode::Double:
			{
				warpDouble(vertex, noiseX, noiseZ);
				break;
			}
			case WarpMode::None:
			default:
			{
				break;
			}
		}

		for (int i = 0; i < octaves; i++)
		{
			float n = noise->Get(noiseX * frequency, noiseZ * frequency);
			n = 1.0f - std::abs(n);
			n *= n;

			totalNoise += n * amplitude * prev;
			prev = n;

			frequency *= lacunarity;
			amplitude *= persistence;
		}

		vertex.position.y = totalNoise;
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

		float noiseX = vertex.position.x * scale;
		float noiseZ = vertex.position.z * scale;

		switch (warpMode)
		{
			case WarpMode::Single:
			{
				warpSingle(vertex, noiseX, noiseZ);
				break;
			}
			case WarpMode::Double:
			{
				warpDouble(vertex, noiseX, noiseZ);
				break;
			}
			case WarpMode::None:
			default:
			{
				break;
			}
		}

		for (int i = 0; i < octaves; i++)
		{
			float n = noise->Get(noiseX * frequency, noiseZ * frequency);
			totalNoise += n * amplitude;

			frequency *= lacunarity;
			amplitude *= persistence;
		}

		vertex.position.y = totalNoise;
	}
}

void TerrainGenerator::warpSingle(Vertex& v, float& wx, float& wz)
{
	warp(v, wx, wz, this->config.warpFrequency, this->config.warpMultiplier);
}

void TerrainGenerator::warpDouble(Vertex& v, float& wx, float& wz)
{
	warp(v, wx, wz, this->config.warpFrequency, this->config.warpMultiplier);
	warp(v, wx, wz, this->config.warpFrequency * 2.0f, this->config.warpMultiplier * 0.5f);
}

void TerrainGenerator::warp(Vertex& v, float& wx, float& wz, float frequency, float multiplier)
{
	float eps = 0.001f;

	float noise_dx_plus = noise->Get((wx + eps) * frequency, wz * frequency);
	float noise_dx_minus = noise->Get((wx - eps) * frequency, wz * frequency);
	float dNoise_dx = (noise_dx_plus - noise_dx_minus) / (2.0f * eps);

	float noise_dz_plus = noise->Get(wx * frequency, (wz + eps) * frequency);
	float noise_dz_minus = noise->Get(wx * frequency, (wz - eps) * frequency);
	float dNoise_dz = (noise_dz_plus - noise_dz_minus) / (2.0f * eps);

	wx = wx + (dNoise_dz * multiplier);
	wz = wz + (-dNoise_dx * multiplier);
}

void TerrainGenerator::applyErosion()
{
	int vertCountX = static_cast<unsigned int>(config.width / config.resolution) + 1;
	int vertCountZ = static_cast<unsigned int>(config.depth / config.resolution) + 1;
	std::vector<float> heightMap;
	
	for (Vertex& v : terrainMesh->GetVertices())
	{
		heightMap.push_back(v.position.y);
	}
	
	int erosionIterations = 500;
	float talus = 1.0f;
	float erosionStrength = 0.5f;

	for (int i = 0; i < erosionIterations; i++)
	{
		std::vector<float> erosionMap(heightMap.size(), 0.0f);
		for (int z = 0; z < vertCountZ - 1; z++)
		{
			for (int x = 0; x < vertCountX - 1; x++)
			{
				int index = z * vertCountX + x;
				float currentHeight = heightMap[index];
			}
		}
	}
}