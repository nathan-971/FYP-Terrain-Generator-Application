#ifndef _HEIGHT_MAP_PANEL_H_
#define _HEIGHT_MAP_PANEL_H_

#include <glad/glad.h>
#include "UI/panels/ipanel.h"

class HeightMapPanel : public IPanel
{
public:
	void Display(EditorContext& ctx) override;

private:
	unsigned int textureID = 0;
	int generateHeightMapTexture(EditorContext& ctx);
};

#endif