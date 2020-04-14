#ifndef TERRAIN_CONFIG_H
#define TERRAIN_CONFIG_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "engine/prehistoric/core/model/material/Material.h"

#include "engine/prehistoric/core/util/Util.h"
#include "engine/prehistoric/core/math/Math.h"

namespace TerrainConfig
{
	void LoadConfig(const std::string& path, Window* window);
	int UpdateMorphingArea(int lod);

	extern float scaleY;
	extern float scaleXZ;

	extern int tessellationFactor;
	extern float tessellationSlope;
	extern float tessellationShift;

	extern std::vector<int> lodRanges;
	extern std::vector<int> lodMorphingAreas;

	extern std::string heightmap;
	extern std::vector<Material*> materials;
};

#endif