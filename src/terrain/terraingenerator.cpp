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

    for (auto& vertex : terrainMesh->GetVertices())
    {
        float slope = terrainMesh->GetSlopeAt(
            vertex.position.x,
            vertex.position.z,
            this->config.width,
            this->config.depth,
            this->config.resolution
        );

        glm::vec3 grassColor(0.1f, 0.6f, 0.1f);
        glm::vec3 dirtColor(0.45f, 0.35f, 0.2f);
        glm::vec3 rockColor(0.5f, 0.5f, 0.5f);
        glm::vec3 snowColor(0.9f, 0.9f, 0.9f);

        if (slope < 25.0f)
        {
            vertex.color = grassColor;
        }
        else if (slope < 45.0f)
        {
            float t = (slope - 25.0f) / 20.0f;
            vertex.color = glm::mix(grassColor, dirtColor, t);
        }
        else if (slope < 70.0f)
        {
            float t = (slope - 45.0f) / 25.0f;
            vertex.color = glm::mix(dirtColor, rockColor, t);
        }
        else
        {
            float t = (slope - 70.0f) / 20.0f;
            t = glm::clamp(t, 0.0f, 1.0f);
            vertex.color = glm::mix(rockColor, snowColor, t);
        }
    }

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