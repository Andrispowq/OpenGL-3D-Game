#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include <fstream>

#include "engine/prehistoric/core/util/Util.h"
#include "engine/prehistoric/core/math/Math.h"

namespace EngineConfig
{
    void LoadConfig(const std::string& path);

    extern unsigned int lightsMaxNumber;
    extern float rendererGamma;
    extern int rendererHighDetailRange;
    extern float rendererNearPlane;
    extern float rendererFarPlane;
};

#endif