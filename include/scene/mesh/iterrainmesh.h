#ifndef _I_TERRAIN_MESH_H_
#define _I_TERRAIN_MESH_H_

#include "terrain/heightmap.h"

#include "scene/mesh/vertex.h"
#include "scene/mesh/transform.h"

class ITerrainMesh
{
public:
	virtual ~ITerrainMesh() = default;
	virtual void UpdateBuffers() = 0;
	virtual void ApplyHeightMap(HeightMap& map) = 0;
	virtual void Create(unsigned int width, unsigned int depth, float resolution) = 0;
	virtual void RecalculateNormals() = 0;

	virtual const std::vector<Vertex>& GetVertices() const = 0;
	virtual const std::vector<unsigned int>& GetIndices() const = 0;
	virtual const Transform& getTransform() const = 0;
	virtual Transform& getTransform() = 0;
	virtual int GetVertexXCount() = 0;
	virtual const int GetVertexXCount() const = 0;
	virtual int GetVertexZCount() = 0;
	virtual const int GetVertexZCount() const = 0;
};

#endif