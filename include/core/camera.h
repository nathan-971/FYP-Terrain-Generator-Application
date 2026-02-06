#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "core/time.h"
#include "renderer/shader.h"

class Camera
{
public:
	Camera(int width, int height, glm::vec3 position);
	void updateCameraMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Inputs(GLFWwindow* window);
	void onResize(int newWidth, int newHeight);

	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view;
	glm::mat4 projection;

	bool firstClick = true;
	int width;
	int height;
	float speed = 5.0f;
	float sensitivity = 100.0f;
};

#endif