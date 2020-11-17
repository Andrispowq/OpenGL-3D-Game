#ifndef ENVIROMENT_MAP_CONFIG_H
#define ENVIROMENT_MAP_CONFIG_H

#include <fstream>

#include "engine/prehistoric/core/util/Util.h"

namespace EnviromentMapConfig
{
    void LoadConfig(const std::string& path);

    extern std::string enviromentMapLocation;
    extern uint32_t enviromentMapResolution;
    extern uint32_t irradianceMapResolution;
    extern uint32_t prefilterMapResolution;
    extern uint32_t prefilterLevels;
};

#endif