#ifndef _I_SHADER_H_
#define _I_SHADER_H_

#include <glm/glm.hpp>

class IShader
{
protected:
    IShader() = default;

public:
    virtual ~IShader() = default;

    virtual void Load(const char* vertexSource, const char* fragmentSource) = 0;
    virtual void Activate() const = 0;
    virtual void setUniformFloat(const char* name, float f) = 0;
    virtual void setUniformInt(const char* name, int i) = 0;
    virtual void setUniformVec3(const char* name, const glm::vec3& vec) = 0;
    virtual void setUniformMat(const char* name, const glm::mat4& mat) = 0;
};

#endif