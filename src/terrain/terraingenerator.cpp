#include "terrain/terraingenerator.h"
#include "noise/perlinnoise.h"

#include <iostream>
#include <random>

TerrainGenerator::TerrainGenerator(TerrainConfig& config) :
	config(config), 
	noise(nullptr), 
	terrainMesh(nullptr), 
	shaderProgram(0),
	seed(0),
	noiseConfig(NoiseConfiguration::BaseNoise), 
	warpMode(WarpMode::None),
	erosionEnabled(false)
{
	noise = new PerlinNoise();
}

TerrainGenerator::~TerrainGenerator()
{
	if (noise)
	{
		delete noise;
		noise = nullptr;
	}
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

	if (erosionEnabled)
	{
		applyHydraulicErosion();
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

		float angle = 0.785398f; //45 Degrees In Radians
		float cosOfAngle = cos(angle);
		float sinOfAngle = sin(angle);

		float rotatedX = vertex.position.x * cosOfAngle - vertex.position.z * sinOfAngle;
		float rotatedZ = vertex.position.x * sinOfAngle + vertex.position.z * cosOfAngle;

		float noiseX = rotatedX * scale;
		float noiseZ = rotatedZ * scale;


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

		float angle = 0.785398f; //45 Degrees In Radians
		float cosOfAngle = cos(angle);
		float sinOfAngle = sin(angle);

		float rotatedX = vertex.position.x * cosOfAngle - vertex.position.z * sinOfAngle;
		float rotatedZ = vertex.position.x * sinOfAngle + vertex.position.z * cosOfAngle;

		float noiseX = rotatedX * scale;
		float noiseZ = rotatedZ * scale;


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

void TerrainGenerator::applyHydraulicErosion()
{
	const int lifetime = 20;
	const float inertia = 0.3f;
	const float capacityFactor = 1.5f;
	const float minCapacity = 0.01f;
	const float erodeSpeed = 0.08f;
	const float depositSpeed = 0.3f;
	const float evaporation = 0.005f;

	const float minHeight = 0.1f;
	const float maxDeltaHeight = 1.0f;
	const float maxErodePerStep = 0.5f;
	const float maxDepositPerStep = 0.5f;
	
	int numDroplets = 70000;

	int vertCountX = static_cast<int>(config.width / config.resolution) + 1;
	int vertCountZ = static_cast<int>(config.depth / config.resolution) + 1;

	std::vector<float> heightMap(vertCountX * vertCountZ);
	for (int i = 0; i < heightMap.size(); i++)
	{
		heightMap[i] = terrainMesh->GetVertices()[i].position.y;
	}

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> distX(0.0f, static_cast<float>(vertCountX - 2));
	std::uniform_real_distribution<float> distZ(0.0f, static_cast<float>(vertCountZ - 2));

	for (int i = 0; i < numDroplets; i++)
	{
		Droplet drop;
		drop.posX = distX(rng);
		drop.posZ = distZ(rng);

		for (int step = 0; step < lifetime; step++)
		{
			drop.posX = std::clamp(drop.posX, 0.0f, static_cast<float>(vertCountX - 2));
			drop.posZ = std::clamp(drop.posZ, 0.0f, static_cast<float>(vertCountZ - 2));

			int x = static_cast<int>(drop.posX);
			int z = static_cast<int>(drop.posZ);

			float fx = drop.posX - x;
			float fz = drop.posZ - z;

			float h00 = getHeight(heightMap, x, z, vertCountX);
			float h10 = getHeight(heightMap, x + 1, z, vertCountX);
			float h01 = getHeight(heightMap, x, z + 1, vertCountX);
			float h11 = getHeight(heightMap, x + 1, z + 1, vertCountX);

			float heightCenter =
				h00 * (1 - fx) * (1 - fz) + h10 * fx * (1 - fz) + h01 * (1 - fx) * fz + h11 * fx * fz;

			// Compute gradient
			float gradX = (h10 - h00) * (1 - fz) + (h11 - h01) * fz;
			float gradZ = (h01 - h00) * (1 - fx) + (h11 - h10) * fx;

			// Update droplet direction
			drop.dirX = drop.dirX * inertia - gradX * (1 - inertia);
			drop.dirZ = drop.dirZ * inertia - gradZ * (1 - inertia);

			// Normalize direction
			float len = std::sqrt(drop.dirX * drop.dirX + drop.dirZ * drop.dirZ);
			if (len != 0.0f) 
			{
				drop.dirX /= len;
				drop.dirZ /= len;
			}

			// Move droplet scaled by speed
			drop.posX += drop.dirX * drop.speed;
			drop.posZ += drop.dirZ * drop.speed;

			// Sample new height
			float newHeight = sampleHeight(heightMap, drop.posX, drop.posZ, vertCountX, vertCountZ);
			float deltaHeight = newHeight - heightCenter;

			// Compute sediment capacity
			float capacity = std::max(-deltaHeight * drop.speed * drop.water * capacityFactor, minCapacity);

			// Erosion or deposition
			if (drop.sediment > capacity || deltaHeight > 0.0f)
			{
				// Deposit sediment
				float depositAmount = (deltaHeight > 0.0f) ? std::min(deltaHeight, drop.sediment)
					: (drop.sediment - capacity) * depositSpeed;
				depositAmount = std::clamp(depositAmount, 0.0f, maxDepositPerStep);
				drop.sediment -= depositAmount;

				// Distribute deposition bilinearly
				h00 += depositAmount * (1 - fx) * (1 - fz);
				h10 += depositAmount * fx * (1 - fz);
				h01 += depositAmount * (1 - fx) * fz;
				h11 += depositAmount * fx * fz;

				// Write back to heightMap
				getHeight(heightMap, x, z, vertCountX) = h00;
				getHeight(heightMap, x + 1, z, vertCountX) = h10;
				getHeight(heightMap, x, z + 1, vertCountX) = h01;
				getHeight(heightMap, x + 1, z + 1, vertCountX) = h11;
			}
			else
			{
				// Erode sediment
				float erodeAmount = (capacity - drop.sediment) * erodeSpeed;
				erodeAmount = std::clamp(erodeAmount, 0.0f, maxErodePerStep);
				drop.sediment += erodeAmount;

				// Distribute erosion bilinearly, clamp to minHeight
				h00 = std::max(h00 - erodeAmount * (1 - fx) * (1 - fz), minHeight);
				h10 = std::max(h10 - erodeAmount * fx * (1 - fz), minHeight);
				h01 = std::max(h01 - erodeAmount * (1 - fx) * fz, minHeight);
				h11 = std::max(h11 - erodeAmount * fx * fz, minHeight);

				getHeight(heightMap, x, z, vertCountX) = h00;
				getHeight(heightMap, x + 1, z, vertCountX) = h10;
				getHeight(heightMap, x, z + 1, vertCountX) = h01;
				getHeight(heightMap, x + 1, z + 1, vertCountX) = h11;
			}
			drop.water *= (1.0f - evaporation);
		}
	}

	for (int i = 0; i < heightMap.size(); ++i)
	{
		terrainMesh->GetVertices()[i].position.y = heightMap[i];
	}
}

float TerrainGenerator::sampleHeight(const std::vector<float>& heightMap, float x, float z, int width, int depth)
{
	x = std::clamp(x, 0.0f, static_cast<float>(width - 2));
	z = std::clamp(z, 0.0f, static_cast<float>(depth - 2));

	int ix = static_cast<int>(x);
	int iz = static_cast<int>(z);
	float fx = x - ix;
	float fz = z - iz;

	int ix1 = ix + 1;
	int iz1 = iz + 1;

	float h00 = heightMap[iz * width + ix];
	float h10 = heightMap[iz * width + ix1];
	float h01 = heightMap[iz1 * width + ix];
	float h11 = heightMap[iz1 * width + ix1];

	return h00 * (1 - fx) * (1 - fz) +
		h10 * fx * (1 - fz) +
		h01 * (1 - fx) * fz +
		h11 * fx * fz;
}

float& TerrainGenerator::getHeight(std::vector<float>& heightMap, int x, int z, int width)
{
	return heightMap[z * width + x];
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

void TerrainGenerator::setErosionEnabled(bool& erosionEnabled)
{
	this->erosionEnabled = erosionEnabled;
}

TerrainConfig& TerrainGenerator::getConfig()
{
	return this->config;
}