#ifndef _MESH_H_
#define _MESH_H_

#include "scene/mesh/meshbase.h"
#include "scene/mesh/iterrainmesh.h"
#include "scene/mesh/vertex.h"
#include "scene/mesh/transform.h"

#include "terrain/heightmap.h"

class TerrainMesh : public MeshBase, public ITerrainMesh
{
public:
	TerrainMesh();
	~TerrainMesh();

	void UpdateBuffers() override;
	void ApplyHeightMap(HeightMap& map) override;
	void Create(unsigned int width, unsigned int depth, float resolution) override;
	void RecalculateNormals() override;

	const std::vector<Vertex>& GetVertices() const override;
	const std::vector<unsigned int>& GetIndices() const override;
	const Transform& getTransform() const override;
	Transform& getTransform() override;
	int GetVertexXCount() override;
	const int GetVertexXCount() const override;
	int GetVertexZCount() override;
	const int GetVertexZCount() const override;

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