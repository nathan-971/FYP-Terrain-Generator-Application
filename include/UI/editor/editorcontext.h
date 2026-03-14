#ifndef _EDITOR_CONTEXT_H_
#define _EDITOR_CONTEXT_H_

#include "UI/editor/editorstate.h"
#include "UI/editor/editorcommands.h"

#include "terrain/terrainConfig.h"

#include "scene/scene.h"

struct EditorContext
{
	Scene& scene;
	TerrainConfig& terrainConfig;
	EditorState state;
	EditorCommands commands;
};

#endif