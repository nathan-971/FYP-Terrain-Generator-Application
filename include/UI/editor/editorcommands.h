#ifndef _EDITOR_COMMANDS_H_
#define _EDITOR_COMMANDS_H_

struct EditorCommands
{
	bool updateMesh = false;
	bool updateHeightMap = false;
	bool changeWarpMode = false;
	bool changeNoiseConfiguration = false;
	bool changeErosionEnabled = false;
	bool changeSeed = false;

	bool startErosion = false;
	bool stopErosion = false;
	bool resetErosion = false;

	bool changeSkybox = false;
	bool requestExport = false;
};

#endif