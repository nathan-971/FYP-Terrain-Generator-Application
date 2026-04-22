#ifndef _UI_BASE_H_
#define _UI_BASE_H_

#include "core/camera/icamera.h"

#include "exporter/iterrainexporterservice.h"

#include "UI/editor/editor.h"
#include "UI/viewport/viewport.h"

#include "scene/iscene.h"

#include "renderer/iviewportprovider.h"
#include "renderer/ibakedalbedoprovider.h"

class UIBase
{
public:
	UIBase(
		IScene& scene,
		ICamera& camera,
		IViewportProvider& viewportProvider,
		IBakedAlbedoProvider& bakedAlbedoProvider,
		ITerrainExporterService& exporterService
	);
	~UIBase();

	void PreRender();
	void Render();

private:
	void StartRootFrame();
	void LayoutRootFrame();

	IScene& scene;
	ICamera& camera;

	Viewport viewport;
	Editor editor;
};

#endif