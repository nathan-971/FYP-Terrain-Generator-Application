#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform 
{
	glm::vec3 position{ 0 };
	glm::quat rotation{ };
	glm::vec3 scale{ 1 };

	glm::mat4 getMatrix() const
	{
		return glm::translate(glm::mat4(1.0f), position)
			* glm::mat4_cast(rotation)
			* glm::scale(glm::mat4(1.0f), scale);
	}
};

#endif