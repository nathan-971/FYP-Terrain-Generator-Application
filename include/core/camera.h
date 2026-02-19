#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "core/time.h"
#include "renderer/shader.h"

class Camera
{
public:
	Camera(int width, int height, const glm::vec3& position);
	void UpdateCameraMatrix(float FOVdeg, float nearPlane, float farPlane);
	void OnResize(int newWidth, int newHeight);

	void setPosition(const glm::vec3& position);
	void setOrientation(const glm::vec3& orientation);
	
	const glm::vec3& getPosition() const;
	const glm::vec3& getOrientation() const;
	const glm::vec3& getCameraUp() const;

	const glm::mat4& getView() const;
	const glm::mat4& getProjection() const;

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