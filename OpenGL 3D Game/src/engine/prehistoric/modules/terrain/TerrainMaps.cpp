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

	splatMapRenderer = new SplatMapRenderer(window, normalmap->getWidth());
	splatMapRenderer->Render(normalmap);
	this->splatmap = splatMapRenderer->getSplatmap();

	heightsQuery = new TerrainHeightsQuery(window, heightmap->getWidth());
	heightsQuery->Query(heightmap);
	this->terrainHeights = heightsQuery->getHeights();
}

TerrainMaps::~TerrainMaps()
{
	delete heightmap;

	delete normalMapRenderer;
	delete splatMapRenderer;
	delete heightsQuery;
}