#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	~Shader();

	void Load(const char* vertexSource, const char* fragmentSource);
	void Activate() const;
	void setUniformFloat(const char* name, float f);
	void setUniformInt(const char* name, int i);
	void setUniformVec3(const char* name, glm::vec3 vec);
	void setUniformMat(const char* name, glm::mat4 mat);

	unsigned int progID;
private:
	void compileErrors(unsigned int ID, const char* type) const;
	std::string readInShaderFromSource(const char* path) const;
};

#endif