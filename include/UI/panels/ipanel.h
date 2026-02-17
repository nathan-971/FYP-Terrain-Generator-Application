#ifndef _I_PANELS_H_
#define _I_PANELS_H_

#include "imgui/imgui.h"

#include "UI/editor/editorcontext.h"

class IPanel
{
public:
	virtual ~IPanel() = default;
	virtual void Display(EditorContext& ctx) = 0;
};

#endif