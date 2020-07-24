#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainQuadtree.h"

TerrainQuadtree::TerrainQuadtree(Window* window, AssetManager* manager, Camera* camera, TerrainMaps* maps)
	: window(window), camera(camera), maps(maps)
{
	size_t shaderID = -1;
	Pipeline* pipeline = nullptr;
	size_t wireframeShaderID = -1;
	Pipeline* wireframePipeline = nullptr;

	PatchVertexBuffer* vbo = nullptr;

	if (FrameworkConfig::api == OpenGL)
	{
		vbo = new GLPatchVertexBuffer();
		vbo->Store(generatePatch());
		size_t vboID = manager->addVertexBuffer(vbo);

		shaderID = manager->addShader(new GLTerrainShader());
		wireframeShaderID = manager->addShader(new GLTerrainWireframeShader());

		pipeline = new GLGraphicsPipeline(manager, shaderID, vboID);
		wireframePipeline = new GLGraphicsPipeline(manager, wireframeShaderID, vboID);

		reinterpret_cast<GLGraphicsPipeline*>(pipeline)->SetBackfaceCulling(true);
		reinterpret_cast<GLGraphicsPipeline*>(wireframePipeline)->SetBackfaceCulling(true);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//TODO: implement terrains in Vulkan
	}

	pipeline->CreatePipeline(window);
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