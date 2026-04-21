#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "scene/texture/texture.h"
#include <memory>

struct Material
{
    std::shared_ptr<Texture> albedo;
    std::shared_ptr<Texture> normal;
    std::shared_ptr<Texture> roughness;
};

#endif