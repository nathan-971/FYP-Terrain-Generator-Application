#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "renderer/shader.h"

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view;
	glm::mat4 projection;

	bool firstClick = true;
	int width;
	int height;
	float deltaTime;
	float speed = 5.0f;
	float sensitivity = 100.0f;

	Camera(int width, int height, glm::vec3 position);
	void updateCameraMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Inputs(GLFWwindow* window);
	void onResize(int newWidth, int newHeight);
};

#endif