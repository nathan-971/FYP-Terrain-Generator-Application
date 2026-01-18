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
	Mesh();
	~Mesh();

	void UpdateBuffers();
	void Create(unsigned int width, unsigned int depth, float resolution);
	void Draw(unsigned int& shaderProgram);
	void recalculateNormals(unsigned int width, unsigned int depth, float resolution);
	float GetSlopeAt(unsigned int x, unsigned int z, unsigned int width, unsigned int depth, float resolution);
	
	std::vector<Vertex>& GetVertices();
	std::vector<unsigned int>& GetIndices();
private:
	void buildMesh(unsigned int width, unsigned int depth, float resolution);

	bool finished;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO, VBO, EBO;
};

#endif