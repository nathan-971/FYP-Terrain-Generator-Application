#include "renderer/shader.h"
#include <iostream>

Shader::Shader() 
	: progID(0) { }

Shader::~Shader()
{
	glDeleteProgram(progID);
}

void Shader::Load(const char* vertexSource, const char* fragmentSource)
{
	std::string vertexCode = readInShaderFromSource(vertexSource);
	std::string fragmentCode = readInShaderFromSource(fragmentSource);

	const char* cStringVertex = vertexCode.c_str();
	const char* cStringFrag = fragmentCode.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cStringVertex, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cStringFrag, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	progID = glCreateProgram();
	glAttachShader(progID, vertexShader);
	glAttachShader(progID, fragmentShader);
	glLinkProgram(progID);
	compileErrors(progID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() const
{
	glUseProgram(progID);
}

void Shader::setUniformFloat(const char* name, float f)
{
	glUniform1f(glGetUniformLocation(progID, name), f);
}

void Shader::setUniformInt(const char* name, int i)
{
	glUniform1i(glGetUniformLocation(progID, name), i);
}

void Shader::setUniformVec3(const char* name, glm::vec3 vec)
{
	glUniform3f(glGetUniformLocation(progID, name), vec.x, vec.y, vec.z);
}

void Shader::setUniformMat(const char* name, glm::mat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(progID, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::compileErrors(unsigned int ID, const char* type) const
{
	int hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << " SHADER\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << infoLog << std::endl;
		}
	}
}

std::string Shader::readInShaderFromSource(const char* path) const
{
	std::string contents = "";

	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << path << " Failed To Open" << std::endl;
		return "";
	}

	std::string line;
	while (std::getline(file, line))
	{
		contents += line + "\n";
	}
	file.close();

	return contents;
}