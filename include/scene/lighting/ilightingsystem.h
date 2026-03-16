#ifndef _I_LIGHTING_SYSTEM_H_
#define _I_LIGHTING_SYSTEM_H_

#include "scene/lighting/light.h"

#include <glm/glm.hpp>

class ILightingSystem
{
public:
	virtual ~ILightingSystem() = default;

	virtual void setLightPositon(glm::vec3 position) = 0;
	virtual void setLightColor(glm::vec3 color) = 0;
	virtual void setLightAmbient(float ambient) = 0;
	virtual void setLightSpecular(float specular) = 0;
	virtual void setLightShininess(int shine) = 0;

	virtual Light& getLight() = 0;
	virtual const Light& getLight() const = 0;
};

#endif
