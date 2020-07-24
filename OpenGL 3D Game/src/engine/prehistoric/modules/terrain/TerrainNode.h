#ifndef TERRAIN_NODE_H
#define TERRAIN_NODE_H

#include "engine/prehistoric/core/gameObject/GameObject.h"

#include "TerrainMaps.h"

#include "engine/prehistoric/common/buffer/PatchVertexBuffer.h"
#include "engine/prehistoric/core/math/Math.h"

#include "engine/prehistoric/component/renderer/Renderer.h"

class TerrainNode : public GameObject
{
public:
	TerrainNode(Pipeline* pipeline, Pipeline* wireframePipeline, TerrainMaps* maps,
		Window* window, Camera* camera, const Vector2f& location,
		int lod, const Vector2f& index);
	virtual ~TerrainNode();

	void PreRender(RenderingEngine* renderingEngine) override;

	void UpdateQuadtree();
	void UpdateChildNodes();

	void AddChildNodes(int lod);
	void RemoveChildNodes();

	void ComputeWorldPosition();
	float getTerrainHeight(const Vector2f& location) const;

	TerrainMaps* getMaps() const { return maps; }

	int getLod() const { return lod; }
	Vector2f getLocation() const { return location; }
	Vector3f getWorldPosition() const { return worldPosition; }
	Vector2f getIndex() const { return index; }
	float getGap() const { return gap; }

	Transform* getLocalTransform() const { return localTransform; }
private:
	Window* window;
	Camera* camera;
	
	TerrainMaps* maps;

	bool leaf;
	int lod;
	Vector2f location;
	Vector3f worldPosition;
	Vector2f index;
	float gap;

	Renderer* renderer;
	Renderer* wireframeRenderer;
	Material* material;

	Transform* localTransform;
};

#endif