#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/glm.hpp>

struct Light
{
    glm::vec3 position;
    glm::vec3 color;
    float ambient;
    float specular;
    int shininess;
};

#endif