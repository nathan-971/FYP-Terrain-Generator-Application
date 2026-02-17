#ifndef _EROSION_PANEL_H_
#define _EROSION_PANEL_H_

#include "UI/panels/ipanel.h"
#include "UI/editor/editorcontext.h"

class ErosionPanel : public IPanel
{
public:
	void Display(EditorContext& ctx);
};

#endif