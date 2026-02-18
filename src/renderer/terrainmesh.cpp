#include "renderer/terrainmesh.h"

TerrainMesh::TerrainMesh() 
	: finished(false), transform() { }

TerrainMesh::~TerrainMesh() { }

std::vector<Vertex>& TerrainMesh::GetVertices()
{
	return this->vertices;
}

std::vector<unsigned int>& TerrainMesh::GetIndices()
{
	return this->indices;
}

Transform& TerrainMesh::getTransform()
{
	return this->transform;
}

void TerrainMesh::Create(unsigned int width, unsigned int depth, float resolution)
{
	buildMesh(width, depth, resolution);
	UpdateBuffers();
}

void TerrainMesh::UpdateBuffers()
{
	glBindVertexArray(VAO);

	uploadVertexData(vertices.data(), vertices.size(), GL_STATIC_DRAW);
	uploadIndicesData(indices.data(), indices.size(), GL_STATIC_DRAW);

	enableAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
	enableAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
	enableAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
}

void TerrainMesh::buildMesh(unsigned int width, unsigned int depth, float resolution)
{
	vertices.clear();
	indices.clear();

	unsigned int vertCountX = static_cast<unsigned int>(width / resolution) + 1;
	unsigned int vertCountZ = static_cast<unsigned int>(depth / resolution) + 1;

	vertices.reserve(vertCountX * vertCountZ);

	float xSpacing = static_cast<float>(width) / (vertCountX - 1);
	float zSpacing = static_cast<float>(depth) / (vertCountZ - 1);

	for (int x = 0; x < vertCountX; x++)
	{
		for (int z = 0; z < vertCountZ; z++)
		{
			Vertex v;
			v.position = glm::vec3(
				x * xSpacing - (width * 0.5f),
				0.0f,
				z * zSpacing - (depth * 0.5f)
			);
			v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			v.color = glm::vec3(0.0f, 0.0f, 1.0f);
			vertices.push_back(v);
		}
	}

	for (int x = 0; x < vertCountX - 1; x++)
	{
		for (int z = 0; z < vertCountZ - 1; z++)
		{
			unsigned int topLeft = x * vertCountZ + z;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = (x + 1) * vertCountZ + z;
			unsigned int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
}

void TerrainMesh::recalculateNormals(unsigned int width, unsigned int depth, float resolution)
{
	unsigned int vertCountX = static_cast<unsigned int>(width / resolution) + 1;
	unsigned int vertCountZ = static_cast<unsigned int>(depth / resolution) + 1;

	for (int x = 0; x < vertCountX; x++)
	{
		for (int z = 0; z < vertCountZ; z++)
		{
			int xLeft = (x - 1 < 0) ? 0 : x - 1;
			int xRight = (x + 1 >= vertCountX) ? vertCountX - 1 : x + 1;
			int zDown = (z - 1 < 0) ? 0 : z - 1;
			int zUp = (z + 1 >= vertCountZ) ? vertCountZ - 1 : z + 1;

			float heightLeft = vertices[xLeft * vertCountZ + z].position.y;
			float heightRight = vertices[xRight * vertCountZ + z].position.y;
			float heightDown = vertices[x * vertCountZ + zDown].position.y;
			float heightUp = vertices[x * vertCountZ + zUp].position.y;

			glm::vec3 normal;
			normal.x = heightLeft - heightRight;
			normal.y = 1.0f;
			normal.z = heightDown - heightUp;

			vertices[x * vertCountZ + z].normal = glm::normalize(normal);
		}
	}
}