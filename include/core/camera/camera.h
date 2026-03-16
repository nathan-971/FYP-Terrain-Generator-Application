#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "core/camera/icamera.h"

class Camera : public ICamera
{
public:
	Camera(int width, int height, const glm::vec3& position);
	void UpdateCameraMatrix(float FOVdeg, float nearPlane, float farPlane) override;
	void OnResize(int newWidth, int newHeight) override;

	void setPosition(const glm::vec3& position) override;
	void setOrientation(const glm::vec3& orientation) override;
	
	const glm::vec3& getPosition() const override;
	const glm::vec3& getOrientation() const override;
	const glm::vec3& getCameraUp() const override;

	const glm::mat4& getView() const override;
	const glm::mat4& getProjection() const override;

private:
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 up;

	glm::mat4 view;
	glm::mat4 projection;

	int width;
	int height;
};

#endif