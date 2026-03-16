#ifndef _EDITOR_CONTEXT_H_
#define _EDITOR_CONTEXT_H_

#include "UI/editor/editorstate.h"
#include "UI/editor/editorcommands.h"

#include "scene/skybox/skyboxconfig.h"
#include "terrain/terrainConfig.h"

#include "scene/iscene.h"

struct EditorContext
{
	IScene& scene;
	TerrainConfig& terrainConfig;
	SkyboxConfig& skyboxConfig;

	EditorState state;
	EditorCommands commands;
};

#endif