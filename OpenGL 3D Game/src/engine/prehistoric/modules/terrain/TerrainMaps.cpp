#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainMaps.h"

TerrainMaps::TerrainMaps(Window* window)
{
	this->window = window;

	heightmap = TextureLoader::LoadTexture(TerrainConfig::heightmap, window);

	//TODO: normal + splatmap
	normalmap = nullptr;
	splatmap = nullptr;
}

TerrainMaps::~TerrainMaps()
{
	delete heightmap;

	//TODO: normal + splatmap
	delete normalmap;
	delete splatmap;
}