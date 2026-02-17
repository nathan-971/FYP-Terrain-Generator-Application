#ifndef _EXPORT_PANEL_H_
#define _EXPORT_PANEL_H_

#include "UI/panels/ipanel.h"
#include "UI/editor/editorcontext.h"
#include "IconsFontAwesome6.h"

class ExportPanel : public IPanel
{
public:
	void Display(EditorContext& ctx);
};

#endif