#ifndef _SHADER_H_
#define _SHADER_H_

#include <glm/glm.hpp>
#include <string>

#include "renderer/ishader.h"

class Shader : public IShader
{
public:
	Shader();
	~Shader();

	void Load(const char* vertexSource, const char* fragmentSource) override;
	void Activate() const override;
	void setUniformFloat(const char* name, float f) override;
	void setUniformInt(const char* name, int i) override;
	void setUniformVec3(const char* name, const glm::vec3& vec) override;
	void setUniformMat(const char* name, const glm::mat4& mat) override;

private:
	void compileErrors(unsigned int ID, const char* type) const;
	std::string readInShaderFromSource(const char* path) const;

	unsigned int progID;
};

#endif