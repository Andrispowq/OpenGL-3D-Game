#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainNode.h"

#include "TerrainQuadtree.h"

TerrainNode::TerrainNode(Factory<TerrainNode>* factory, Pipeline* pipeline, Pipeline* wireframePipeline, TerrainMaps* maps,
	Window* window, Camera* camera, const Vector2f& location,
	int lod, const Vector2f& index)
	: factory(factory), maps(maps), window(window), camera(camera), location(location), lod(lod), index(index)
{
	this->gap = 1.0f / float(TerrainQuadtree::rootNodes * pow(2, lod));

	Vector3f localScaling = { gap, 0, gap };
	Vector3f localPosition = { location.x, 0, location.y };

	localTransform = new Transform();

	localTransform->setScaling(localScaling);
	localTransform->setPosition(localPosition);

	worldTransform->setScaling({ TerrainConfig::scaleXZ, TerrainConfig::scaleY, TerrainConfig::scaleXZ });
	worldTransform->setPosition({ -TerrainConfig::scaleXZ / 2.0f, 0, -TerrainConfig::scaleXZ / 2.0f });

	renderer = new Renderer(pipeline, nullptr, window);
	wireframeRenderer = new Renderer(wireframePipeline, nullptr, window);

	AddComponent(RENDERER_COMPONENT, renderer);
	AddComponent(WIREFRAME_RENDERER_COMPONENT, wireframeRenderer);

	ComputeWorldPosition();
	UpdateQuadtree();
}

void TerrainNode::Init(Factory<TerrainNode>* factory, Pipeline* pipeline, Pipeline* wireframePipeline, TerrainMaps* maps,
	Window* window, Camera* camera, const Vector2f& location,
	int lod, const Vector2f& index)
{
	this->factory = factory;

	this->maps = maps;
	this->window = window;
	this->camera = camera;
	
	this->location = location;
	this->lod = lod;
	this->index = index;

	this->gap = 1.0f / float(TerrainQuadtree::rootNodes * pow(2, lod));

	Vector3f localScaling = { gap, 0, gap };
	Vector3f localPosition = { location.x, 0, location.y };

	localTransform = new Transform();

	localTransform->setScaling(localScaling);
	localTransform->setPosition(localPosition);

	worldTransform->setScaling({ TerrainConfig::scaleXZ, TerrainConfig::scaleY, TerrainConfig::scaleXZ });
	worldTransform->setPosition({ -TerrainConfig::scaleXZ / 2.0f, 0, -TerrainConfig::scaleXZ / 2.0f });

	renderer = new Renderer(pipeline, nullptr, window);
	wireframeRenderer = new Renderer(wireframePipeline, nullptr, window);

	AddComponent(RENDERER_COMPONENT, renderer);
	AddComponent(WIREFRAME_RENDERER_COMPONENT, wireframeRenderer);

	ComputeWorldPosition();
	UpdateQuadtree();
}

TerrainNode::~TerrainNode()
{
}

void TerrainNode::PreRender(RenderingEngine* renderingEngine)
{
	if (leaf)
	{
		if (renderingEngine->isWireframeMode())
		{
			wireframeRenderer->PreRender(renderingEngine);
		}
		else
		{
			renderer->PreRender(renderingEngine);
		}
	}

	for (auto child : children)
	{
		child.second->PreRender(renderingEngine);
	}
}

void TerrainNode::UpdateQuadtree()
{
	UpdateChildNodes();

	if (children.size() != 0)
	{
		for (auto child : children)
		{
			((TerrainNode*)child.second)->UpdateQuadtree();
		}
	}
}

void TerrainNode::UpdateChildNodes()
{
	float distance = (camera->getPosition() - worldPosition).length();

	if (distance < TerrainConfig::lodRanges[lod])
	{
		AddChildNodes(lod + 1);
	}
	else
	{
		RemoveChildNodes();
	}
}

void TerrainNode::AddChildNodes(int lod)
{
	if (leaf)
	{
		leaf = false;
	}

	if (children.size() == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				std::stringstream ss;
				ss << "Child ";
				ss << i;
				ss << ", ";
				ss << j;
				ss << ", lod: ";
				ss << lod;

				AddChild(ss.str(), new/*(*factory)*/ TerrainNode(factory, renderer->getPipeline(), wireframeRenderer->getPipeline(), maps,
					window, camera, location + Vector2f(float(i), float(j)) * (gap / 2.f), lod, { float(i), float(j) }));
			}
		}
	}
}

void TerrainNode::RemoveChildNodes()
{
	if (!leaf)
	{
		leaf = true;
	}

	if (children.size() != 0)
	{
		for (auto& child : children)
		{
			delete child.second;
			//TerrainNode::operator delete(child.second, *factory);
		}

		children.clear();
	}
}

void TerrainNode::ComputeWorldPosition()
{
	Vector2f loc = (location + gap / 2.0f) * TerrainConfig::scaleXZ - TerrainConfig::scaleXZ / 2.0f;
	float height = getTerrainHeight(loc);
	this->worldPosition = { loc.x, height, loc.y };
}

float TerrainNode::getTerrainHeight(const Vector2f& location) const
{
	float height = 0;

	Vector2f position = location;
	position += TerrainConfig::scaleXZ / 2.0f;
	position /= TerrainConfig::scaleXZ;

	Texture* heightmap = maps->getHeightmap();
	uint32_t width = heightmap->getWidth();
	float* heights = maps->getHeights();

	Vector2f floor = { std::floor(position.x), std::floor(position.y) };
	position -= floor;
	position *= float(width);

	int x0 = (int) std::floor(position.x);
	int x1 = x0 + 1;
	int z0 = (int)std::floor(position.y);
	int z1 = z0 + 1;

	float h0 = heights[width * z0 + x0];
	float h1 = heights[width * z0 + x1];
	float h2 = heights[width * z1 + x0];
	float h3 = heights[width * z1 + x1];

	float percentU = position.x - x0;
	float percentV = position.y - z0;

	float dU, dV;
	if (percentU > percentV)
	{ //bottom triangle
		dU = h1 - h0;
		dV = h3 - h1;
	}
	else
	{
		dU = h3 - h2;
		dV = h2 - h0;
	}

	height = h0 + (dU * percentU) + (dV * percentV);
	height *= TerrainConfig::scaleY;

	return height;
}