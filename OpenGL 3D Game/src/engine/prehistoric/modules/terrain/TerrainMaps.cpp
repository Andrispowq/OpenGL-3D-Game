#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainMaps.h"

#include "engine/prehistoric/resources/AssetManager.h"

TerrainMaps::TerrainMaps(Window* window, AssetManager* manager)
{
	this->heightmap = manager->getResourceByID<Texture>(manager->getResource<Texture>(TerrainConfig::heightmap));

 	this->normalmapRendererComponent = new NormalMapRenderer(window, manager, 60, heightmap->getWidth());
	normalmapRendererComponent->Render(heightmap);
	this->normalmap = normalmapRendererComponent->getNormalmap();

	this->splatmapRendererComponent = new SplatMapRenderer(window, manager, normalmap->getWidth());
	splatmapRendererComponent->Render(normalmap);
	this->splatmap = splatmapRendererComponent->getSplatmap();

	this->query = new TerrainHeightsQuery(window, manager, heightmap->getWidth());
	query->Query(heightmap);
	this->heights = query->getHeights();
}

TerrainMaps::~TerrainMaps()
{
	delete normalmapRendererComponent;
	delete splatmapRendererComponent;
	delete query;
}
