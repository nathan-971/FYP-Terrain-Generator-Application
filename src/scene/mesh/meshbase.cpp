#include "scene/mesh/meshbase.h"

MeshBase::MeshBase()
	: VAO(0), VBO(0), EBO(0), vertexCount(0), indicesCount(0), indexed(false)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

MeshBase::~MeshBase()
{
	if (EBO) 
	{
		glDeleteBuffers(1, &EBO);
	}
	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
	}
	if (VAO)
	{
		glDeleteVertexArrays(1, &VAO);
	}
}

void MeshBase::uploadVertexData(Vertex* vertices, int size, int drawType)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), vertices, drawType);

	vertexCount = size;
}

void MeshBase::uploadIndicesData(unsigned int* indicies, int size, int drawType)
{
	if (!EBO)
	{
		glGenBuffers(1, &EBO);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indicies, drawType);

	indicesCount = size;
	indexed = true;
}

void MeshBase::enableAttribPointer(unsigned int index, int size, int dataType, bool normalized, int sizeOfDataType, int offset)
{
	glVertexAttribPointer(index, size, dataType, normalized, sizeOfDataType, (void*)offset);
	glEnableVertexAttribArray(index);
}

void MeshBase::Draw() const
{
	glBindVertexArray(VAO);

	if (indexed)
	{
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
}