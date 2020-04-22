#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainQuadtree.h"

#include "TerrainNode.h"

int TerrainQuadtree::rootNodes = 8;
std::vector<Vector2f> TerrainQuadtree::vertices;

TerrainQuadtree::TerrainQuadtree(TerrainMaps& maps, Window* window, Camera* camera)
{
	this->maps = &maps;
	this->window = window;
	this->camera = camera;

	if (vertices.size() == 0)
	{
		vertices = GeneratePatch();
	
		if (FrameworkConfig::api == OpenGL)
		{
			mesh = new GLPatchVBO();
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			//TODO
		}

		mesh->Store(vertices);
	}

	material = new Material(window);

	Shader* shader = nullptr;

	if (FrameworkConfig::api == OpenGL)
	{
		shader = new GLTerrainWireframeShader();
		pipeline = new GLPipeline(shader, mesh);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		pipeline = new VKPipeline(shader, mesh);
	}

	pipeline->CreatePipeline(window);

	for (int x = 0; x < rootNodes; x++)
	{
		for (int y = 0; y < rootNodes; y++)
		{
			std::string name = "Node ";
			name.append(std::to_string(x));
			name.append(", ");
			name.append(std::to_string(y));

			AddChild(name, new TerrainNode(this, { (float) x / (float) rootNodes, (float) y / (float) rootNodes }, { (float) x, (float) y }, 0));
		}
	}

	worldTransform->SetScaling({ TerrainConfig::scaleXZ, TerrainConfig::scaleY, TerrainConfig::scaleXZ });
	worldTransform->SetPosition({ TerrainConfig::scaleXZ / 2, 0, TerrainConfig::scaleXZ / 2 });
}

TerrainQuadtree::~TerrainQuadtree()
{
}

void TerrainQuadtree::UpdateQuadtree()
{
	for (auto child : children)
	{
		((TerrainNode*)child.second)->UpdateQuadtree();
	}
}

std::vector<Vector2f> TerrainQuadtree::GeneratePatch()
{
	std::vector<Vector2f> patches;
	patches.reserve(16);

	size_t index = 0;

	vertices.push_back({ 0, 0 });
	vertices.push_back({ 0.333f, 0 });
	vertices.push_back({ 0.666f, 0 });
	vertices.push_back({ 1, 0 });

	vertices.push_back({ 0, 0.333f });
	vertices.push_back({ 0.333f, 0.333f });
	vertices.push_back({ 0.666f, 0.333f });
	vertices.push_back({ 1, 0.333f });

	vertices.push_back({ 0, 0.666f });
	vertices.push_back({ 0.333f, 0.666f });
	vertices.push_back({ 0.666f, 0.666f });
	vertices.push_back({ 1, 0.666f });

	vertices.push_back({ 0, 1 });
	vertices.push_back({ 0.333f, 1 });
	vertices.push_back({ 0.666f, 1 });
	vertices.push_back({ 1, 1 });

	return vertices;
}