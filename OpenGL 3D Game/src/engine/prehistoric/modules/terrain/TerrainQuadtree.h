#ifndef TERRAIN_QUADTREE_H
#define TERRAIN_QUADTREE_H

#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/gameObject/Node.h"

#include "engine/platform/opengl/buffer/GLPatchVBO.h"
//#include "engine/platform/vulkan/buffer/VKPatchVBO.h"

#include "TerrainMaps.h"

#include "engine/prehistoric/component/renderer/Renderer.h"
#include "engine/prehistoric/common/framework/Window.h"
#include "engine/prehistoric/common/buffer/PatchVBO.h"
#include "engine/prehistoric/core/movement/Camera.h"

#include "engine/prehistoric/common/rendering/pipeline/GraphicsPipeline.h"

class TerrainQuadtree : public Node
{
public:
	TerrainQuadtree(TerrainMaps& maps, Window* window, Camera* camera);
	virtual ~TerrainQuadtree();

	void UpdateQuadtree();
	std::vector<Vector2f> GeneratePatch();

	PatchVBO* getVbo() { return mesh; }
	Pipeline* getPipeline() { return pipeline; }
	Pipeline* getWireframePipeline() { return wireframePipeline; }
	Material* getMaterial() { return material; }

	TerrainMaps* getTerrainMaps() { return maps; }
	Window* getWindow() { return window; }
	Camera* getCamera() { return camera; }
public:
	static int rootNodes;
private:
	TerrainMaps* maps;
	Window* window;
	Camera* camera;

	PatchVBO* mesh;
	Pipeline* pipeline;
	Pipeline* wireframePipeline;
	Material* material;

	static std::vector<Vector2f> vertices;
};

#endif