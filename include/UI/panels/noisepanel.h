#ifndef _NOISE_PANEL_H_
#define _NOISE_PANEL_H_

#include <random>

#include "UI/panels/ipanel.h"
#include "UI/editor/editorcontext.h"

class NoisePanel : public IPanel
{
public:
	void Display(EditorContext& ctx) override;

private:
	int generateSeed();
};

#endif