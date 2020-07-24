#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainMaps.h"

TerrainMaps::TerrainMaps(Window* window, AssetManager* manager)
{
	this->heightmap = TextureLoader::LoadTexture(TerrainConfig::heightmap, window, Bilinear);

 	this->normalmapRenderer = new NormalMapRenderer(window, manager, 60, heightmap->getWidth());
	normalmapRenderer->Render(heightmap);
	this->normalmap = normalmapRenderer->getNormalmap();

	this->splatmapRenderer = new SplatMapRenderer(window, manager, normalmap->getWidth());
	splatmapRenderer->Render(normalmap);
	this->splatmap = splatmapRenderer->getSplatmap();

	this->query = new TerrainHeightsQuery(window, manager, heightmap->getWidth());
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
