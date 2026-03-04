#ifndef _MESH_H_
#define _MESH_H_

#include "renderer/meshbase.h"
#include "renderer/vertex.h"
#include "renderer/transform.h"

#include "terrain/heightmap.h"

class TerrainMesh : public MeshBase
{
public:
	TerrainMesh();
	~TerrainMesh();

	void UpdateBuffers();
	void ApplyHeightMap(HeightMap& map);
	void Create(unsigned int width, unsigned int depth, float resolution);
	void RecalculateNormals();

	std::vector<Vertex>& GetVertices();
	std::vector<unsigned int>& GetIndices();
	Transform& getTransform();
	int GetVertexXCount();
	int GetVertexZCount();

private:
	void buildMesh();

	Transform transform;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	bool finished;

	unsigned int width;
	unsigned int depth;
	int vertexCountX;
	int vertexCountZ;
	float resolution;
};

#endif