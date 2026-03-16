#ifndef _MESH_H_
#define _MESH_H_

#include "scene/mesh/meshbase.h"
#include "scene/mesh/vertex.h"
#include "scene/mesh/transform.h"

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

	const std::vector<Vertex>& GetVertices() const;
	const std::vector<unsigned int>& GetIndices() const;
	const Transform& getTransform() const;
	Transform& getTransform();
	int GetVertexXCount();
	const int GetVertexXCount() const;
	int GetVertexZCount();
	const int GetVertexZCount() const;

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