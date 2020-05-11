#ifndef TERRAIN_MAPS_H
#define TERRAIN_MAPS_H

#include "engine/prehistoric/common/model/Texture.h"
#include "engine/prehistoric/common/framework/Window.h"

#include "engine/config/TerrainConfig.h"
#include "engine/prehistoric/core/util/loader/TextureLoader.h"

#include "engine/prehistoric/modules/gpgpu/NormalMapRenderer.h"
#include "engine/prehistoric/modules/gpgpu/SplatMapRenderer.h"
#include "engine/prehistoric/modules/gpgpu/TerrainHeightsQuery.h"

class TerrainMaps
{
public:
	TerrainMaps(Window* window);
	virtual ~TerrainMaps();

	Texture* getHeightmap() const { return heightmap; }
	Texture* getNormalmap() const { return normalmap; }
	Texture* getSplatmap() const { return splatmap; }

	float* getTerrainHeights() const { return terrainHeights; }
private:
	Texture* heightmap;
	Texture* normalmap;
	Texture* splatmap;

	NormalMapRenderer* normalMapRenderer;
	SplatMapRenderer* splatMapRenderer;
	TerrainHeightsQuery* heightsQuery;

	Window* window;

	float* terrainHeights;
};

#endif