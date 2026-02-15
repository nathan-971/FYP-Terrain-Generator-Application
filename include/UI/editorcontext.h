#ifndef _EDITOR_CONTEXT_H_
#define _EDITOR_CONTEXT_H_

#include "UI/editorstate.h"
#include "terrain/terrainConfig.h"
#include "renderer/scene.h"

struct EditorContext
{
	Scene& scene;
	TerrainConfig& terrainConfig;
	EditorState state;
};

#endif