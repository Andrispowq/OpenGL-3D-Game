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

	this->leaf = true;

	Vector3f localScaling = { gap, 0, gap };
	Vector3f localPosition = { location.x, 0, location.y };

	localTransform->SetScaling(localScaling);
	localTransform->SetPosition(localPosition);

	worldTransform->SetScaling({ TerrainConfig::scaleXZ, TerrainConfig::scaleY, TerrainConfig::scaleXZ });
	worldTransform->SetPosition({ -TerrainConfig::scaleXZ / 2.f, 0, -TerrainConfig::scaleXZ / 2.f });

	Renderer* renderer = new Renderer(quadtree->getPipeline(), quadtree->getMaterial(), quadtree->getWindow());
	AddComponent(RENDERER_COMPONENT, renderer);

	Renderer* wireframeRenderer = new Renderer(quadtree->getWireframePipeline(), quadtree->getMaterial(), quadtree->getWindow());
	AddComponent(WIREFRAME_RENDERER_COMPONENT, wireframeRenderer);

	ComputeWorldPosition();
	UpdateQuadtree();
}

TerrainNode::~TerrainNode()
{
}

void TerrainNode::PreRender(RenderingEngine* renderingEngine)
{
	//Optimisation: assuming that the terrain does not have any other components
	//the engine only uptades the renderer component, and only does it if it is a leaf node
	if (leaf)
	{
		if (renderingEngine->isWireframeMode())
		{
			GetComponent(RENDERER_COMPONENT)->PreRender(renderingEngine);
		}
		else
		{
			GetComponent(WIREFRAME_RENDERER_COMPONENT)->PreRender(renderingEngine);
		}
	}

	for (auto child : children)
	{
		child.second->PreRender(renderingEngine);
	}
}

void TerrainNode::UpdateQuadtree()
{
	Vector3f start = worldTransform->GetPosition();
	Vector3f end = Vector3f(TerrainConfig::scaleXZ / 2.f, 0, TerrainConfig::scaleXZ / 2.f);

	if (quadtree->getCamera()->getPosition() > start && quadtree->getCamera()->getPosition() < end)
		worldPosition.y = getTerrainHeight({ quadtree->getCamera()->getPosition().x, quadtree->getCamera()->getPosition().z });

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
				std::stringstream name;
				name << "Child ";
				name << i;
				name << ", ";
				name << j;
				name << ", ";
				name << lod;

				AddChild(name.str(), new TerrainNode(quadtree, location + Vector2f((float) i * gap / 2, (float) j * gap / 2), { (float) i, (float) j }, lod));
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

float TerrainNode::getTerrainHeight(Vector2f position) const
{
	float height = 0;

	position -= TerrainConfig::scaleXZ / 2.f;
	Vector2f floor = { (float) std::floor(position.x), (float) std::floor(position.y) };
	position -= floor;
	position *= quadtree->getTerrainMaps()->getHeightmap()->getWidth();
	int x0 = (int)std::floor(position.x);
	int x1 = x0 + 1;
	int z0 = (int)std::floor(position.y);
	int z1 = z0 + 1;

	float h0 = quadtree->getTerrainMaps()->getTerrainHeights()[quadtree->getTerrainMaps()->getHeightmap()->getWidth() * z0 + x0];
	float h1 = quadtree->getTerrainMaps()->getTerrainHeights()[quadtree->getTerrainMaps()->getHeightmap()->getWidth() * z0 + x1];
	float h2 = quadtree->getTerrainMaps()->getTerrainHeights()[quadtree->getTerrainMaps()->getHeightmap()->getWidth() * z1 + x0];
	float h3 = quadtree->getTerrainMaps()->getTerrainHeights()[quadtree->getTerrainMaps()->getHeightmap()->getWidth() * z1 + x1];

	float percentU = position.x - x0;
	float percentV = position.y - z0;

	float dU, dV;

	//bottom (true) or top triangle (false)
	if (percentU > percentV)
	{
		dU = h1 - h0;
		dV = h3 - h1;
	}
	else
	{
		dU = h3 - h2;
		dV = h2 - h0;
	}

	height = h0 + (dU * percentU) + (dV + percentV);
	height *= TerrainConfig::scaleY;

	return height;
}
