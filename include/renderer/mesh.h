#ifndef _MESH_H_
#define _MESH_H_

#include "renderer/meshbase.h"
#include "renderer/vertex.h"

class Mesh : public MeshBase
{
public:
	Mesh();
	~Mesh();

	void UpdateBuffers();
	void Create(unsigned int width, unsigned int depth, float resolution);
	void recalculateNormals(unsigned int width, unsigned int depth, float resolution);
	float GetSlopeAt(unsigned int x, unsigned int z, unsigned int width, unsigned int depth, float resolution);
	
	std::vector<Vertex>& GetVertices();
	std::vector<unsigned int>& GetIndices();
private:
	void buildMesh(unsigned int width, unsigned int depth, float resolution);

	bool finished;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

#endif