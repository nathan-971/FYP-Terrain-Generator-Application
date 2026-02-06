#include "core/camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
	: width(width), height(height), position(position), projection(1.0f), view(1.0f) { }

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

void Camera::Inputs(GLFWwindow* window)
{
	//XZ Coord Controls
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += speed * Time::deltaTime * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += speed * Time::deltaTime * -glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += speed * Time::deltaTime * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += speed * Time::deltaTime * glm::normalize(glm::cross(orientation, up));
	}

	//Y Coord Controls
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		position += speed * Time::deltaTime * up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		position += speed * Time::deltaTime * -up;
	}	
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 25.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 5.0f;
	}

	//Reset Cam
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	//Mouse Controls
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));
		if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			orientation = newOrientation;
		}
		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		glfwSetCursorPos(window, (float)(width / 2), (float)(height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}