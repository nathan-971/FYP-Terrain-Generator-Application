#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint progID;

	Shader(const char* vertexSource, const char* fragmentSource);
	~Shader();
	void Activate() const;
	void setUniformFloat(const char* name, float f);
	void setUniformInt(const char* name, int i);
	void setUniformMat(const char* name, glm::mat4 mat);
private:
	void compileErrors(GLuint ID, const char* type) const;
	std::string readInShaderFromSource(const char* path) const;
};

#endif