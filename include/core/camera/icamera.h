#ifndef _I_CAMERA_H_
#define _I_CAMERA_H_

#include <glm/glm.hpp>

class ICamera
{
public:
	ICamera() = default;
	virtual ~ICamera() = default;

	virtual void UpdateCameraMatrix(float FOVdeg, float nearPlane, float farPlane) = 0;
	virtual void OnResize(int newWidth, int newHeight) = 0;

	virtual void setPosition(const glm::vec3& position) = 0;
	virtual void setOrientation(const glm::vec3& orientation) = 0;

	virtual const glm::vec3& getPosition() const = 0;
	virtual const glm::vec3& getOrientation() const = 0;
	virtual const glm::vec3& getCameraUp() const = 0;

	virtual const glm::mat4& getView() const = 0;
	virtual const glm::mat4& getProjection() const = 0;
};

#endif