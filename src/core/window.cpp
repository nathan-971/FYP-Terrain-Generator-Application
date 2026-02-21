#include "core/window.h"
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window(int width, int height, const std::string& title, bool vsync)
	: width(width), height(height)
{
	initGLFW();
	nativeWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (nativeWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwSetWindowUserPointer(nativeWindow, this);

	glfwMakeContextCurrent(this->nativeWindow);
	glfwSwapInterval(vsync);
}

Window::~Window()
{
	glfwDestroyWindow(this->nativeWindow);
	glfwTerminate();
}

void Window::initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_NUM);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_NUM);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int Window::getWidth() const
{
	return this->width;
}

int Window::getHeight() const
{
	return this->height;
}

GLFWwindow* Window::getNativeWindow() const
{
	return this->nativeWindow;
}

void Window::setUserPointer(void* object) const
{
	glfwSetWindowUserPointer(this->nativeWindow, object);
}

void Window::pollEvents() const
{
	glfwPollEvents();
}

void Window::swapBuffers() const
{
	glfwSwapBuffers(this->nativeWindow);
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(this->nativeWindow);
}

double Window::getTime() const
{
	return glfwGetTime();
}

void Window::getMousePosition(double& x, double& y)
{
	return glfwGetCursorPos(nativeWindow, &x, &y);
}
