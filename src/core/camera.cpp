#include "core/camera.h"

Camera::Camera(int width, int height, glm::vec3 position) : 
	width(width), 
	height(height), 
	position(position),
	orientation(glm::vec3(0.0f)),
	up(glm::vec3(0.0f, 1.0f, 0.0f)),
	projection(1.0f),
	view(1.0f) { }

void Camera::updateCameraMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	view = glm::lookAt(position, position + orientation, up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
}

void Camera::onResize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}

void Camera::setOrientation(glm::vec3 orientation)
{
	this->orientation = orientation;
}

void Camera::setPosition(glm::vec3 position)
{
	this->position = position;
}

glm::vec3& Camera::getPosition()
{
	return this->position;
}

glm::vec3& Camera::getOrientation()
{
	return this->orientation;
}

glm::vec3& Camera::getCameraUp()
{
	return this->up;
}

glm::mat4& Camera::getView()
{
	return this->view;
}

glm::mat4& Camera::getProjection()
{
	return this->projection;
}