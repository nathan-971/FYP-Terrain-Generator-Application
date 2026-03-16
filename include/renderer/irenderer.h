#ifndef _I_RENDERER_H_
#define _I_RENDERER_H_

#include "core/camera/icamera.h"
#include "scene/iscene.h"
#include "renderer/framedata.h"


class IRenderer
{
public:
	virtual ~IRenderer() = default;
	virtual void RenderScene(const FrameData& frameData) = 0;
	virtual FrameData getFrameData(const ICamera& camera, const IScene& scene) = 0;
};

#endif