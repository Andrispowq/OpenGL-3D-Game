#ifndef TERRAIN_NODE_H
#define TERRAIN_NODE_H

#include "engine/prehistoric/core/gameObject/GameObject.h"

#include "TerrainMaps.h"

#include "engine/prehistoric/component/renderer/Renderer.h"
#include "engine/prehistoric/common/framework/Window.h"
#include "engine/prehistoric/core/movement/Camera.h"

#include "engine/platform/opengl/buffer/GLPatchVBO.h"
//#include "engine/platform/vulkan/buffer/VKPatchVBO.h"

#include "engine/platform/opengl/rendering/pipeline/GLPipeline.h"
#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"

#include "engine/platform/opengl/rendering/shaders/terrain/GLTerrainShader.h"
//#include "engine/platform/vulkan/rendering/shaders/terrain/VKTerrainShader.h"

#include "engine/prehistoric/common/rendering/RenderingEngine.h"

#include "engine/config/FrameworkConfig.h"

class TerrainNode : public GameObject
{
public:
	TerrainNode(TerrainMaps& maps, Window* window, Camera* camera, const std::vector<Vector2f>& vertices, Vector2f location, Vector2f index, int lod);
	virtual ~TerrainNode();

	virtual void PreRender(RenderingEngine* renderingEngine) override;

	void UpdateQuadtree();
	void AddChildNodes(int lod);
	void RemoveChildNodes();
	void UpdateChildNodes();

	void ComputeWorldPosition();

	TerrainMaps* getMaps() const { return maps; }

	Vector3f getWorldPosition() const { return worldPosition; }
	Vector2f getLocation() const { return location; }
	Vector2f getIndex() const { return index; }

	float getGap() const { return gap; }
	int getLod() const { return lod; }
	bool isLeaf() const { return leaf; }
private:
	TerrainMaps* maps; //Per-terrain data, stored in Terrain class and also deleted there
	Window* window;
	Camera* camera;

	PatchVBO* mesh;
	std::vector<Vector2f> vertices;

	Vector3f worldPosition;
	Vector2f location;
	Vector2f index;

	float gap;
	int lod;
	bool leaf;

	static Material* material;
	static Pipeline* pipeline;
};

#endif