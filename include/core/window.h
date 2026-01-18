#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GL_VERSION_NUM 3

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
	Window(int height, int width, const std::string& title, bool vsync);
	~Window();

	int getWidth() const;
	int getHeight() const;
	GLFWwindow* getNativeWindow() const;

	void setUserPointer(void* object) const;
	void pollEvents() const;
	void swapBuffers() const;
	int shouldClose() const;
	double getTime() const;

	void updateViewport(int width, int height);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
private:
	GLFWwindow* nativeWindow;
	int width;
	int height;

	void initGLFW();
	void initGLAD();
};

#endif