#ifndef _H_MESH_BASE_
#define _H_MESH_BASE_

#include "renderer/vertex.h"

#include <glad/glad.h>
#include <vector>

class MeshBase
{
public:
	MeshBase();
	virtual ~MeshBase();

	void Draw() const;

protected:
	void uploadVertexData(Vertex* vertices, int size, int drawType);
	void uploadIndicesData(unsigned int* indices, int size, int drawType);
	void enableAttribPointer(unsigned int index, int size, int dataType, bool normalized, int sizeOfDataType, int offset);

	int vertexCount;
	int indicesCount;
	unsigned int VAO, VBO, EBO;
	bool indexed;
};

#endif