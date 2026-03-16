#ifndef _LIGHTING_SYSTEM_H_
#define _LIGHTING_SYSTEM_H_

#include "scene/lighting/ilightingsystem.h"
#include "scene/lighting/light.h"

class LightingSystem : public ILightingSystem
{
public:
	LightingSystem();
	~LightingSystem();

	void setLightPositon(glm::vec3 position) override;
	void setLightColor(glm::vec3 color) override;
	void setLightAmbient(float ambient) override;
	void setLightSpecular(float specular) override;
	void setLightShininess(int shine) override;

	const Light& getLight() const override;
	Light& getLight() override;

private:
	Light light;
};

#endif