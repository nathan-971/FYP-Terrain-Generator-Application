#ifndef _BAKED_ALBEDO_PROVIDER_H_
#define _BAKED_ALBEDO_PROVIDER_H_

#include "scene/iscene.h"

class IBakedAlbedoProvider
{
public:
	virtual ~IBakedAlbedoProvider() = default;
	virtual unsigned int getBakedAlbedoTexture() const = 0;
	virtual int getBakedAlbedoWidth() const = 0;
	virtual int getBakedAlbedoHeight() const = 0;
	virtual void RenderFinishedAlbedoToTexture(const IScene& scene) = 0;
};

#endif