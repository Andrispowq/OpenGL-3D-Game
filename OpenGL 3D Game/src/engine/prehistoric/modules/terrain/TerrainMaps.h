#ifndef TERRAIN_MAPS_H
#define TERRAIN_MAPS_H

#include "engine/prehistoric/common/model/Texture.h"
#include "engine/prehistoric/common/framework/Window.h"

#include "engine/config/TerrainConfig.h"
#include "engine/prehistoric/core/util/loader/TextureLoader.h"

class TerrainMaps
{
public:
	TerrainMaps(Window* window);
	virtual ~TerrainMaps();

	Texture* getHeightmap() const { return heightmap; }
	Texture* getNormalmap() const { return normalmap; }
	Texture* getSplatmap() const { return splatmap; }
private:
	Texture* heightmap;
	Texture* normalmap;
	Texture* splatmap;

	Window* window;
};

#endif