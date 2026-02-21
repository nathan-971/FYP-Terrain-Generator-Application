#ifndef _H_GRAPHICSCONTEXT_H_
#define _H_GRAPHICSCONTEXT_H_

#include "core/iwindow.h"

class GraphicsContext
{
public:
	GraphicsContext(const IWindow& window);
	~GraphicsContext();
	void Init();

private:
	const IWindow& window;
};

#endif