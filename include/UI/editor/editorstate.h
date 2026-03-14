#ifndef _EDITOR_STATE_H_
#define _EDITOR_STATE_H_

#include "terrain/noise/noiseconfiguration.h"
#include "terrain/warp/warpmode.h"

#include "scene/skybox/skyboxoption.h"

struct EditorState
{
    NoiseConfiguration noiseConfig = NoiseConfiguration::BaseNoise;
    WarpMode warpMode = WarpMode::None;
	SkyboxOption skyboxOption = SkyboxOption::NOON;
    bool erosionEnabled = false;
    int seed = 0;
    int fps = 0;
};

#endif