#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainNode.h"

#include "engine/prehistoric/core/util/Constants.h"

TerrainNode::TerrainNode(TerrainQuadtree* quadtree, Vector2f location, Vector2f index, int lod)
{
	this->quadtree = quadtree;

	this->location = location;
	this->index = index;
	this->lod = lod;
	this->gap = float(1 / (TerrainQuadtree::rootNodes * pow(2, lod)));

	Vector3f localScaling = { gap, 0, gap };
	Vector3f localPosition = { location.x, 0, location.y };

	localTransform->SetScaling(localScaling);
	localTransform->SetPosition(localPosition);

	worldTransform->SetScaling({ TerrainConfig::scaleXZ, TerrainConfig::scaleY, TerrainConfig::scaleXZ });
	worldTransform->SetPosition({ -TerrainConfig::scaleXZ / 2, 0, -TerrainConfig::scaleXZ / 2 });

	Renderer* renderer = new Renderer(quadtree->getPipeline(), quadtree->getMaterial(), quadtree->getWindow());
	AddComponent(RENDERER_COMPONENT, renderer);

	ComputeWorldPosition();
	UpdateQuadtree();
}

TerrainNode::~TerrainNode()
{
}

void TerrainNode::PreRender(RenderingEngine* renderingEngine)
{
	//Optimisation: assuming that the terrain does not have any other components (which is weird, why would a terrain have a light component for instance???)
	//the engine only uptades the renderer component, and only does it if it is a leaf node
	if (leaf)
	{
		GetComponent(RENDERER_COMPONENT)->PreRender(renderingEngine);
	}

	for (auto child : children)
	{
		child.second->PreRender(renderingEngine);
	}

	GameObject::PreRender(renderingEngine);
}

void TerrainNode::UpdateQuadtree()
{
	if (quadtree->getCamera()->getPosition().y > TerrainConfig::scaleY)
	{
		worldPosition.y = TerrainConfig::scaleY;
	}
	else
	{
		worldPosition.y = quadtree->getCamera()->getPosition().y;
	}

	UpdateChildNodes();

	for (auto child : children)
	{
		((TerrainNode*)child.second)->UpdateQuadtree();
	}
}

void TerrainNode::AddChildNodes(int lod)
{
	/*if (lod >= TerrainConfig::lodRanges.size())
		return;*/

	if (leaf)
		leaf = false;

	if (children.size() == 0)
	{
		for (unsigned int i = 0; i < 2; i++)
		{
			for (unsigned int j = 0; j < 2; j++)
			{
				std::string name = "Child ";
				name.append(std::to_string(i));
				name.append(", ");
				name.append(std::to_string(j));
				name.append(", ");
				name.append(std::to_string(lod));

				AddChild(name, new TerrainNode(quadtree, { (float) i * gap / 2, (float) j * gap / 2 }, { (float) i, (float) j }, lod));
			}
		}
	}
}

void TerrainNode::RemoveChildNodes()
{
	if (!leaf)
		leaf = true;

	if (children.size() != 0)
	{
		for (auto& child : children)
		{
			delete child.second;
		}

		children.clear();
	}
}

void TerrainNode::UpdateChildNodes()
{
	float dist = (quadtree->getCamera()->getPosition() - worldPosition).length();

	if (dist < TerrainConfig::lodRanges[lod])
	{
		AddChildNodes(lod + 1);
	}
	else if (dist >= TerrainConfig::lodRanges[lod])
	{
		RemoveChildNodes();
	}
}

void TerrainNode::ComputeWorldPosition()
{
	Vector2f loc = (location + (gap / 2)) * TerrainConfig::scaleXZ - TerrainConfig::scaleXZ / 2;
	worldPosition = { loc.x, 0, loc.y };
}
