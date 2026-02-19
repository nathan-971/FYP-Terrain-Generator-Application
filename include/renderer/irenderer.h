#ifndef _I_RENDERER_H_
#define _I_RENDERER_H_

#include <core/camera.h>
#include <renderer/scene.h>

class IRenderer
{
public:
	virtual ~IRenderer() = default;
	virtual void RenderScene(const FrameData& frameData) = 0;
};

#endif