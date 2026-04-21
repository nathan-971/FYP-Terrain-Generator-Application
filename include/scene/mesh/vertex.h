#ifndef _H_VERTEX_
#define _H_VERTEX_

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

#endif