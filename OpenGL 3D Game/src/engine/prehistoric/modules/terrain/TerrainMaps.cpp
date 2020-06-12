#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainMaps.h"

TerrainMaps::TerrainMaps(Window* window)
{
	this->heightmap = TextureLoader::LoadTexture(TerrainConfig::heightmap, window, Bilinear);

 	this->normalmapRenderer = new NormalMapRenderer(window, 60, heightmap->getWidth());
	normalmapRenderer->Render(heightmap);
	this->normalmap = normalmapRenderer->getNormalmap();

	this->splatmapRenderer = new SplatMapRenderer(window, normalmap->getWidth());
	splatmapRenderer->Render(normalmap);
	this->splatmap = splatmapRenderer->getSplatmap();

	this->query = new TerrainHeightsQuery(window, heightmap->getWidth());
	query->Query(heightmap);
	this->heights = query->getHeights();
}

TerrainMaps::~TerrainMaps()
{
	delete heightmap;
	delete normalmapRenderer;
	delete splatmapRenderer;
	delete query;
}
