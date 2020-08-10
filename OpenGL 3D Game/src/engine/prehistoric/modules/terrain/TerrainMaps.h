#ifndef TERRAIN_MAPS_H
#define TERRAIN_MAPS_H

#include "engine/config/TerrainConfig.h"

#include "engine/platform/opengl/texture/GLTexture.h"
#include "engine/platform/vulkan/texture/VKTexture.h"

#include "engine/prehistoric/modules/gpgpu/NormalMapRendererComponent.h"
#include "engine/prehistoric/modules/gpgpu/SplatMapRendererComponent.h"
#include "engine/prehistoric/modules/gpgpu/TerrainHeightsQuery.h"

class TerrainMaps
{
public:
	TerrainMaps(Window* window, AssetManager* manager);
	virtual ~TerrainMaps();

	Texture* getHeightmap() const { return heightmap; }
	Texture* getNormalmap() const { return normalmap; }
	Texture* getSplatmap() const { return splatmap; }

	float* getHeights() const { return heights; }
private:
	Window* window;

	Texture* heightmap;
	Texture* normalmap;
	Texture* splatmap;

	float* heights;

	NormalMapRendererComponent* normalmapRendererComponent;
	SplatMapRendererComponent* splatmapRendererComponent;
	TerrainHeightsQuery* query;
};

#endif