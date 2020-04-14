#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainNode.h"

#include "TerrainQuadtree.h"
#include "engine/prehistoric/core/util/Constants.h"

Material* TerrainNode::material = nullptr;
Pipeline* TerrainNode::pipeline = nullptr;

TerrainNode::TerrainNode(TerrainMaps& maps, Window* window, Camera* camera, const std::vector<Vector2f>& vertices, Vector2f location, Vector2f index, int lod)
{
	this->maps = &maps;
	this->window = window;
	this->camera = camera;

	this->location = location;
	this->index = index;
	this->lod = lod;
	this->gap = float(1 / (TerrainQuadtree::rootNodes * pow(2, lod)));

	if (FrameworkConfig::api == OpenGL)
	{
		mesh = new GLPatchVBO();
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		PR_LOG_RUNTIME_ERROR("ERROR: Create VKPatchVBO!!!!! Source: TerrainNode.\n");
	}

	mesh->Store(vertices);

	Vector3f localScaling = { gap, 0, gap };
	Vector3f localPosition = { location.x, 0, location.y };

	localTransform->SetScaling(localScaling);
	localTransform->SetPosition(localPosition);

	worldTransform->SetScaling({ TerrainConfig::scaleXZ, TerrainConfig::scaleY, TerrainConfig::scaleXZ });
	worldTransform->SetPosition({ -TerrainConfig::scaleXZ / 2, 0, -TerrainConfig::scaleXZ / 2 });

	//TODO: material is per-terrain, it will not be static
	if (material == nullptr)
	{
		material = new Material(window);
	}

	PR_LOG_MESSAGE("hello\n");
	if (pipeline == nullptr)
	{
		Shader* shader = nullptr;

		if (FrameworkConfig::api == OpenGL)
		{
			shader = new GLTerrainShader();
			pipeline = new GLPipeline(shader);
		}
		else if (FrameworkConfig::api == Vulkan)
		{

			pipeline = new VKPipeline(shader);
		}
	}
	PR_LOG_MESSAGE("hello\n");

	Renderer* renderer = new Renderer(mesh, pipeline, material, window);
	AddComponent(RENDERER_COMPONENT, renderer);
}

TerrainNode::~TerrainNode()
{
	delete mesh;
}

void TerrainNode::PreRender(RenderingEngine* renderingEngine)
{
	//Optimisation: assuming that the terrain does not have any other components (which is weird, why would a terrain have a light component for instance???)
	//the engine only uptades the renderer component, and only does it if it is a leaf node
	if (leaf)
	{
		GetComponent(RENDERER_COMPONENT)->PreRender(renderingEngine);
	}
	else
	{
		for (auto child : children)
		{
			child.second->PreRender(renderingEngine);
		}
	}
}

void TerrainNode::UpdateQuadtree()
{
	if (camera->getPosition().y > TerrainConfig::scaleY)
	{
		worldPosition.y = TerrainConfig::scaleY;
	}
	else
	{
		worldPosition.y = camera->getPosition().y;
	}

	UpdateChildNodes();

	for (auto child : children)
	{
		((TerrainNode*)child.second)->UpdateQuadtree();
	}
}

void TerrainNode::AddChildNodes(int lod)
{
	if (leaf)
		leaf = false;

	if (children.size() == 0)
	{
		for (unsigned char i = 0; i < 2; i++)
		{
			for (unsigned char j = 0; j < 2; j++)
			{
				std::string name = "Child ";
				name.append(std::to_string(i));
				name.append(", ");
				name.append(std::to_string(j));
				AddChild(name, new TerrainNode(*maps, window, camera, vertices, { (float) i * gap / 2, (float) j * gap / 2 }, { (float) i, (float) j }, lod));
			}
		}
	}
}

void TerrainNode::RemoveChildNodes()
{
	if (!leaf)
		leaf = true;

	children.clear();
}

void TerrainNode::UpdateChildNodes()
{
	float dist = (camera->getPosition() - worldPosition).length();

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
