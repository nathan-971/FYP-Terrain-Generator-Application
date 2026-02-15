#ifndef _MESH_PANEL_H_
#define _MESH_PANEL_H_

#include "UI/panels/ipanel.h"
#include "UI/editorcontext.h"

class MeshPanel : public IPanel
{
public:
	void Display(EditorContext& ctx) override;
};

#endif