#include "engine/prehistoric/core/util/Includes.hpp"
#include "Terrain.h"

Terrain::Terrain(Window* window, Camera* camera)
{
	this->window = window;
	this->camera = camera;

	maps = new TerrainMaps(window);
	
	AddChild("Quadtree", new TerrainQuadtree(*maps, window, camera));
}

Terrain::~Terrain()
{
	delete maps;
}

void Terrain::UpdateQuadtree()
{
	if (camera->isChanged())
	{
		((TerrainQuadtree*)children["Quadtree"])->UpdateQuadtree();
	}
}

void Terrain::PreRender(RenderingEngine* renderingEngine)
{
	UpdateQuadtree();
	GameObject::PreRender(renderingEngine);
}