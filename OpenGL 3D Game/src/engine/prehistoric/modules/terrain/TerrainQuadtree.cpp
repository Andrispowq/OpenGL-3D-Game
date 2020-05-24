#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainQuadtree.h"

TerrainQuadtree::TerrainQuadtree(Window* window, Camera* camera, TerrainMaps* maps)
	: window(window), camera(camera), maps(maps)
{
	Shader* shader = nullptr;
	Pipeline* pipeline = nullptr;
	Shader* wireframeShader = nullptr;
	Pipeline* wireframePipeline = nullptr;

	PatchVBO* vbo = nullptr;

	if (FrameworkConfig::api == OpenGL)
	{
		vbo = new GLPatchVBO();
		vbo->Store(generatePatch());

		shader = new GLTerrainShader();
		wireframeShader = new GLTerrainWireframeShader();

		pipeline = new GLGraphicsPipeline(shader, vbo);
		wireframePipeline = new GLGraphicsPipeline(wireframeShader, vbo);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//TODO
	}

	pipeline->SetViewportStart({ 0, 0 });
	pipeline->SetViewportSize({ (float) FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight });
	pipeline->SetScissorStart({ 0, 0 });
	pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

	pipeline->CreatePipeline(window);

	wireframePipeline->SetViewportStart({ 0, 0 });
	wireframePipeline->SetViewportSize({ (float)FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight });
	wireframePipeline->SetScissorStart({ 0, 0 });
	wireframePipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

	wireframePipeline->CreatePipeline(window);

	for (int i = 0; i < rootNodes; i++)
	{
		for (int j = 0; j < rootNodes; j++)
		{
			std::stringstream ss;
			ss << "Child ";
			ss << i;
			ss << ", ";
			ss << j;

			AddChild(ss.str(), new TerrainNode(pipeline, wireframePipeline,
				maps, window, camera, { i / (float)rootNodes, j / (float)rootNodes },
				0, { float(i), float(j) }));
		}
	}

	worldTransform->SetScaling({ TerrainConfig::scaleXZ, TerrainConfig::scaleY, TerrainConfig::scaleXZ });
	worldTransform->SetPosition({ -TerrainConfig::scaleXZ / 2.0f, 0, -TerrainConfig::scaleXZ / 2.0f });
}

TerrainQuadtree::~TerrainQuadtree()
{
}

void TerrainQuadtree::UpdateQuadtree()
{
	for (auto node : children)
	{
		((TerrainNode*)node.second)->UpdateQuadtree();
	}
}

std::vector<Vector2f> TerrainQuadtree::generatePatch() const
{
	std::vector<Vector2f> vertices;
	vertices.reserve(16);

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