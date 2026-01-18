#include "renderer/mesh.h"

Mesh::Mesh() 
	: VAO(0), VBO(0), EBO(0), finished(false) { }

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
}

std::vector<Vertex>& Mesh::GetVertices()
{
	return this->vertices;
}

std::vector<unsigned int>& Mesh::GetIndices()
{
	return this->indices;
}

void Mesh::Create(unsigned int width, unsigned int depth, float resolution)
{
	buildMesh(width, depth, resolution);
	UpdateBuffers();
}

void Mesh::Draw(unsigned int& shaderProgram)
{
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::UpdateBuffers()
{
	if (VAO == 0)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::buildMesh(unsigned int width, unsigned int depth, float resolution)
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
			v.position = glm::vec3(x * xSpacing, 0.0f, z * zSpacing);
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

void Mesh::recalculateNormals(unsigned int width, unsigned int depth, float resolution)
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

float Mesh::GetSlopeAt(unsigned int x, unsigned int z, unsigned int width, unsigned int depth, float resolution)
{
	unsigned int vertCountZ = static_cast<unsigned int>(depth / resolution) + 1;
	glm::vec3 normal = vertices[x * vertCountZ + z].normal;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float slopeRadians = acos(glm::clamp(glm::dot(normal, up), -1.0f, 1.0f));
	return glm::degrees(slopeRadians);
}