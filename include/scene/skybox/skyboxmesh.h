#ifndef _H_SKYBOX_MESH_H_
#define _H_SKYBOX_MESH_H_

#include "scene/mesh/vertex.h"
#include "scene/mesh/meshbase.h"

class SkyboxMesh : public MeshBase
{
public:
	SkyboxMesh();
	~SkyboxMesh();

private:
	std::vector<Vertex> vertices;
	void UpdateBuffers();
};

#endif