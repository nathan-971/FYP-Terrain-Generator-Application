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
	Camera(int width, int height, glm::vec3 position);
	void updateCameraMatrix(float FOVdeg, float nearPlane, float farPlane);
	void onResize(int newWidth, int newHeight);

	void setPosition(glm::vec3 position);
	void setOrientation(glm::vec3 orientation);
	
	glm::vec3& getPosition();
	glm::vec3& getOrientation();
	glm::vec3& getCameraUp();

	glm::mat4& getView();
	glm::mat4& getProjection();

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