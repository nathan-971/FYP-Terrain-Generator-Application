#ifndef _I_VIEWPORT_PROVIDER_H_
#define _I_VIEWPORT_PROVIDER_H_

class IViewportProvider
{
public:
	virtual ~IViewportProvider() = default;
	virtual unsigned int getViewportTexture() const = 0;
	virtual int getViewportWidth() const = 0;
	virtual int getViewportHeight() const = 0;
	virtual void ResizeViewport(int width, int height) = 0;
};

#endif