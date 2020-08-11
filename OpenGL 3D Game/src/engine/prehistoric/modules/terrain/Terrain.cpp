#include "engine/prehistoric/core/util/Includes.hpp"
#include "Terrain.h"

#include "engine/prehistoric/resources/AssembledAssetManager.h"

Terrain::Terrain(Window* window, AssembledAssetManager* manager, Camera* camera)
	: window(window), camera(camera), maps{ std::make_unique<TerrainMaps>(window, manager->getAssetManager()) }
{
	AddChild("Quadtree", new TerrainQuadtree(window, manager, camera, maps.get()));
}

void Terrain::PreRender(Renderer* renderer)
{
	UpdateQuadtree();
	Node::PreRender(renderer);
}

void Terrain::UpdateQuadtree()
{
	if (camera->isChanged())
	{
		((TerrainQuadtree*)children.at("Quadtree"))->UpdateQuadtree();
	}
}
