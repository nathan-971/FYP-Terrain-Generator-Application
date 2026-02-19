#include "core/camera.h"

Camera::Camera(int width, int height, const glm::vec3& position) :
	width(width), 
	height(height), 
	position(position),
	orientation(glm::vec3(0.0f)),
	up(glm::vec3(0.0f, 1.0f, 0.0f)),
	projection(1.0f),
	view(1.0f) { }

void Camera::UpdateCameraMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	view = glm::lookAt(position, position + orientation, up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
}

void Camera::OnResize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}

void Camera::setOrientation(const glm::vec3& orientation)
{
	this->orientation = orientation;
}

void Camera::setPosition(const glm::vec3& position)
{
	this->position = position;
}

const glm::vec3& Camera::getOrientation() const
{
	return this->orientation;
}

const glm::vec3& Camera::getCameraUp() const
{
	return this->up;
}

const glm::vec3& Camera::getPosition() const
{
	return this->position;
}

const glm::mat4& Camera::getView() const
{
	return this->view;
}

const glm::mat4& Camera::getProjection() const
{
	return this->projection;
}