#ifndef _SKYBOX_PANEL_H_
#define _SKYBOX_PANEL_H_

#include "UI/panels/ipanel.h"
#include "UI/editor/editorcontext.h"
#include "IconsFontAwesome6.h"

class SkyboxPanel : public IPanel
{
public:
	void Display(EditorContext& ctx);
};

#endif