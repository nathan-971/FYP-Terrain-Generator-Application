#ifndef _MESH_H_
#define _MESH_H_

#include "renderer/meshbase.h"
#include "renderer/vertex.h"
#include "renderer/transform.h"

class TerrainMesh : public MeshBase
{
public:
	TerrainMesh();
	~TerrainMesh();

	void UpdateBuffers();
	void Create(unsigned int width, unsigned int depth, float resolution);
	void recalculateNormals(unsigned int width, unsigned int depth, float resolution);

	std::vector<Vertex>& GetVertices();
	std::vector<unsigned int>& GetIndices();
	Transform& getTransform();

private:
	void buildMesh(unsigned int width, unsigned int depth, float resolution);

	Transform transform;

	bool finished;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

#endif