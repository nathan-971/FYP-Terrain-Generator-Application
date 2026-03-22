#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GL_VERSION_NUM 3

#include <string>
#include "core/iwindow.h"

class Window : public IWindow
{
public:
	Window(int width, int height, const std::string& title, bool vsync);
	~Window();

	int getWidth() const;
	int getHeight() const;

	GLFWwindow* getNativeWindow() const override;
	void pollEvents() const override;
	void swapBuffers() const override;
	bool shouldClose() const override;
	double getTime() const override;

	void setIcon(const std::string& iconpath) override;
	void setUserPointer(void* object) const;
	void getMousePosition(double &x, double &y);

private:
	void initGLFW();

	GLFWwindow* nativeWindow;
	int width;
	int height;
};

#endif