#include "engine/prehistoric/core/util/Includes.hpp"
#include "Terrain.h"

Terrain::Terrain(Window* window, Camera* camera)
	: window(window), camera(camera)
{
	maps = new TerrainMaps(window);
	
	AddChild("Quadtree", new TerrainQuadtree(window, camera, maps));
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
