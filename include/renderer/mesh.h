#ifndef _MESH_H_
#define _MESH_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
};

class Mesh
{
public:
	Mesh(unsigned int width, unsigned int depth, float resolution, unsigned int shaderProgram);
	~Mesh();

	void createMesh(unsigned int width, unsigned int depth, float resolution);
	std::vector<Vertex>& GetVertices();
	std::vector<unsigned int>& GetIndices();
	void Draw(unsigned int& shaderProgram);
	void UpdateBuffers();
	void recalculateNormals(unsigned int width, unsigned int depth, float resolution);
	float GetSlopeAt(unsigned int x, unsigned int z, unsigned int width, unsigned int depth, float resolution);
private:
	bool finished;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO, VBO, EBO;
};

#endif