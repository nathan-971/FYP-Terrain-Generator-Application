#ifndef _H_SKYBOX_MESH_H_
#define _H_SKYBOX_MESH_H_

#include "renderer/vertex.h"
#include "renderer/meshbase.h"

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