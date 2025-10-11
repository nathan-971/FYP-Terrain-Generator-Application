#include "renderer/mesh.h"
#include <iostream>

Mesh::Mesh(unsigned int width, unsigned int depth, float resolution, unsigned int shaderProgram) :
	VAO(0), VBO(0), EBO(0), finished(false)
{
	this->finished = createMesh(width, depth, resolution);
	if (finished)
	{
		UpdateBuffers();
	}
}

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

bool Mesh::createMesh(unsigned int width, unsigned int depth, float resolution)
{
	vertices.clear();
	indices.clear();

	unsigned int vertCountX = static_cast<unsigned int>(width / resolution) + 1;
	unsigned int vertCountZ = static_cast<unsigned int>(depth / resolution) + 1;

	vertices.reserve(vertCountX * vertCountZ);

	float xSpacing = static_cast<float>(width) / (vertCountX - 1);
	float zSpacing = static_cast<float>(depth) / (vertCountZ - 1);

	for (int z = 0; z < vertCountZ; z++)
	{
		for (int x = 0; x < vertCountX; x++)
		{
			Vertex v;
			v.position = glm::vec3(x * xSpacing, 0.0f, z * zSpacing);
			v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			v.color = glm::vec3(0.0f, 0.0f, 1.0f);
			vertices.push_back(v);
		}
	}

	for (int z = 0; z < vertCountZ - 1; z++)
	{
		for (int x = 0; x < vertCountX - 1; x++)
		{
			unsigned int topLeft = z * vertCountX + x;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = (z + 1) * vertCountX + x;
			unsigned int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
	return true;
}

void Mesh::reMesh(unsigned int width, unsigned int depth, float resolution)
{
	if (createMesh(width, depth, resolution))
	{
		return;
	}
}