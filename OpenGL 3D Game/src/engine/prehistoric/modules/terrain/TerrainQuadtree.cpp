#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainQuadtree.h"

#include "engine/prehistoric/resources/AssembledAssetManager.h"

TerrainQuadtree::TerrainQuadtree(Window* window, AssembledAssetManager* manager, Camera* camera, TerrainMaps* maps)
	: window(window), camera(camera), maps(maps)
{
	factory = new Factory<TerrainNode>(512); //There will definitly not be more than 512 instances of TerrainNode on this quadtree

	size_t shaderID = -1;
	Pipeline* pipeline = nullptr;
	size_t wireframeShaderID = -1;
	Pipeline* wireframePipeline = nullptr;

	PatchVertexBuffer* vbo = nullptr;

	if (FrameworkConfig::api == OpenGL)
	{
		vbo = new GLPatchVertexBuffer(generatePatch());
		size_t vboID = manager->getAssetManager()->addResource<VertexBuffer>(vbo);

		shaderID = manager->getAssetManager()->getResource<Shader>("terrain");
		wireframeShaderID = manager->getAssetManager()->getResource<Shader>("terrain_wireframe");

		pipeline = new GLGraphicsPipeline(window, manager->getAssetManager(), shaderID, vboID);
		wireframePipeline = new GLGraphicsPipeline(window, manager->getAssetManager(), wireframeShaderID, vboID);

		reinterpret_cast<GLGraphicsPipeline*>(pipeline)->SetBackfaceCulling(true);
		reinterpret_cast<GLGraphicsPipeline*>(wireframePipeline)->SetBackfaceCulling(true);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//TODO: implement terrains in Vulkan
	}

	for (int i = 0; i < rootNodes; i++)
	{
		for (int j = 0; j < rootNodes; j++)
		{
			std::stringstream ss;
			ss << "Child ";
			ss << i;
			ss << ", ";
			ss << j;

			AddChild(ss.str(), new/*(*factory)*/ TerrainNode(window, camera, manager, maps, pipeline, wireframePipeline,
				{ i / (float)rootNodes, j / (float)rootNodes },	0, { float(i), float(j) }));
		}
	}

	worldTransform.setScaling({ TerrainConfig::scaleXZ, TerrainConfig::scaleY, TerrainConfig::scaleXZ });
	worldTransform.setPosition({ -TerrainConfig::scaleXZ / 2.0f, 0, -TerrainConfig::scaleXZ / 2.0f });
}

TerrainQuadtree::~TerrainQuadtree()
{
	//Custom allocator -> custom deletion, but the factory deletes them anyway
	//children.clear();

	delete factory;
}

void TerrainQuadtree::UpdateQuadtree()
{
	for (auto& node : children)
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