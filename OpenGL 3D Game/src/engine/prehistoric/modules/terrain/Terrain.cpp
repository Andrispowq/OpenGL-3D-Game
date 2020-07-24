#include "engine/prehistoric/core/util/Includes.hpp"
#include "Terrain.h"

Terrain::Terrain(Window* window, AssetManager* manager, Camera* camera)
	: window(window), camera(camera)
{
	maps = new TerrainMaps(window, manager);
	
	AddChild("Quadtree", new TerrainQuadtree(window, manager, camera, maps));
}

Terrain::~Terrain()
{
	delete maps;
}

void Terrain::PreRender(RenderingEngine* renderingEngine)
{
	UpdateQuadtree();
	Node::PreRender(renderingEngine);
}

void Terrain::UpdateQuadtree()
{
	if (camera->isChanged())
	{
		((TerrainQuadtree*)children.at("Quadtree"))->UpdateQuadtree();
	}
}
