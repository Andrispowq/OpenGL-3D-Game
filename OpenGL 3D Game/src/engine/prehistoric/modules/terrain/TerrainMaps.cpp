#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainMaps.h"

TerrainMaps::TerrainMaps(Window* window, AssetManager* manager)
{
	this->heightmap = TextureLoader::LoadTexture(TerrainConfig::heightmap, window, Bilinear);

 	this->normalmapRendererComponent = new NormalMapRendererComponent(window, manager, 60, heightmap->getWidth());
	normalmapRendererComponent->Render(heightmap);
	this->normalmap = normalmapRendererComponent->getNormalmap();

	this->splatmapRendererComponent = new SplatMapRendererComponent(window, manager, normalmap->getWidth());
	splatmapRendererComponent->Render(normalmap);
	this->splatmap = splatmapRendererComponent->getSplatmap();

	this->query = new TerrainHeightsQuery(window, manager, heightmap->getWidth());
	query->Query(heightmap);
	this->heights = query->getHeights();
}

TerrainMaps::~TerrainMaps()
{
	delete heightmap;
	delete normalmapRendererComponent;
	delete splatmapRendererComponent;
	delete query;
}
