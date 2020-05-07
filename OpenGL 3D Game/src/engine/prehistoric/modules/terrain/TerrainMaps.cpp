#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainMaps.h"

TerrainMaps::TerrainMaps(Window* window)
{
	this->window = window;

	heightmap = TextureLoader::LoadTexture(TerrainConfig::heightmap, window);
	heightmap->SamplerProperties(Bilinear, Repeat);

	normalMapRenderer = new NormalMapRenderer(window, 12.f, heightmap->getWidth());
	normalMapRenderer->Render(heightmap);
	this->normalmap = normalMapRenderer->getNormalmap();
}

TerrainMaps::~TerrainMaps()
{
	delete heightmap;

	delete normalMapRenderer;
}