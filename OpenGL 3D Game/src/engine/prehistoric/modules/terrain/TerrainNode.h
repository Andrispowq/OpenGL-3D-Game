#ifndef TERRAIN_NODE_H
#define TERRAIN_NODE_H

#include "engine/prehistoric/core/gameObject/GameObject.h"

#include "engine/platform/opengl/rendering/pipeline/GLPipeline.h"
#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"

#include "engine/platform/opengl/rendering/shaders/terrain/GLTerrainShader.h"
//#include "engine/platform/vulkan/rendering/shaders/terrain/VKTerrainShader.h"

#include "engine/platform/opengl/rendering/shaders/terrain/GLTerrainWireframeShader.h"
//#include "engine/platform/vulkan/rendering/shaders/terrain/VKTerrainWireframeShader.h"

#include "engine/prehistoric/common/rendering/RenderingEngine.h"

#include "engine/config/FrameworkConfig.h"

#include "TerrainQuadtree.h"

class TerrainNode : public GameObject
{
public:
	TerrainNode(TerrainQuadtree* quatree, Vector2f location, Vector2f index, int lod);
	virtual ~TerrainNode();

	virtual void PreRender(RenderingEngine* renderingEngine) override;

	void UpdateQuadtree();
	void AddChildNodes(int lod);
	void RemoveChildNodes();
	void UpdateChildNodes();

	void ComputeWorldPosition();

	TerrainMaps* getMaps() const { return quadtree->getTerrainMaps(); }

	Vector3f getWorldPosition() const { return worldPosition; }
	Vector2f getLocation() const { return location; }
	Vector2f getIndex() const { return index; }

	float getGap() const { return gap; }
	int getLod() const { return lod; }
	bool isLeaf() const { return leaf; }
private:
	TerrainQuadtree* quadtree;

	Vector3f worldPosition;
	Vector2f location;
	Vector2f index;

	float gap;
	int lod;
	bool leaf;
};

#endif