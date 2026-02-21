#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/graphicscontext.h"
#include <iostream>

GraphicsContext::GraphicsContext(const IWindow& window) : window(window)  { }

GraphicsContext::~GraphicsContext() { }

void GraphicsContext::Init()
{
	static bool initialized = false;
	if (initialized)
	{
		return;
	}

	GLFWwindow* nativeWindow = window.getNativeWindow();
	if (!nativeWindow)
	{
		throw std::runtime_error("Failed to initialize graphics: Window not created");
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize graphics: GLAD failed to initialize");
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	initialized = true;
}