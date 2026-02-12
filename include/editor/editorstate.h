#ifndef _EDITOR_STATE_H_
#define _EDITOR_STATE_H_

#include "renderer/renderer.h"

struct EditorState
{
    NoiseConfiguration noiseConfig = NoiseConfiguration::BaseNoise;
    WarpMode warpMode = WarpMode::None;
    int seed = 0;
    int fps = 0;
};

#endif