#ifndef _LIGHTING_PANEL_H_
#define _LIGHTING_PANEL_H_

#include "UI/panels/ipanel.h"
#include "UI/editorcontext.h"

class LightingPanel : public IPanel
{
public:
	void Display(EditorContext& ctx);
};

#endif