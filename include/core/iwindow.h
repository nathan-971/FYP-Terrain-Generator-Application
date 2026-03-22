#ifndef _I_WINDOW_H_
#define _I_WINDOW_H_

#include <string>

struct GLFWwindow;

class IWindow
{
public:
	IWindow() = default;
	virtual ~IWindow() = default;

	virtual void pollEvents() const = 0;
	virtual void swapBuffers() const = 0;
	virtual bool shouldClose() const = 0;
	virtual void setIcon(const std::string& iconpath) = 0;

	virtual double getTime() const = 0;
	virtual GLFWwindow* getNativeWindow() const = 0;
};

#endif