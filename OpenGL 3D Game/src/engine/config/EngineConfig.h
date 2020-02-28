#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include <fstream>

#include "engine/core/Util.h"
#include "engine/core/math/Math.h"

namespace EngineConfig
{
    void LoadConfig(const std::string& path);
};

#endif