#ifndef TERRAIN_NODE_H
#define TERRAIN_NODE_H

#include "engine/prehistoric/core/gameObject/GameObject.h"

#include "TerrainMaps.h"

#include "engine/prehistoric/common/buffer/PatchVertexBuffer.h"
#include "engine/prehistoric/core/math/Math.h"

#include "engine/prehistoric/component/renderer/RendererComponent.h"

#include "engine/prehistoric/resources/Factory.h"

class TerrainNode : public GameObject
{
public:
	TerrainNode() {}
	TerrainNode(Factory<TerrainNode>* factory, Pipeline* pipeline, Pipeline* wireframePipeline, TerrainMaps* maps,
		Window* window, Camera* camera, const Vector2f& location,
		int lod, const Vector2f& index);
	virtual ~TerrainNode();

	void Init(Factory<TerrainNode>* factory, Pipeline* pipeline, Pipeline* wireframePipeline, TerrainMaps* maps,
		Window* window, Camera* camera, const Vector2f& location,
		int lod, const Vector2f& index);

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

	//Overloaded new and delete comes here:
	void* operator new(size_t size, Factory<TerrainNode>& factory)
	{
		return factory.allocate();
	}

	void* operator new(size_t size)
	{
		void* ptr = malloc(size);
		return ptr;
	}

	void operator delete(void* ptr, Factory<TerrainNode>& factory)
	{
		TerrainNode* _ptr = (TerrainNode*)ptr;
		_ptr->~TerrainNode();
		factory.deallocate(_ptr);
	}

	void operator delete(void* ptr)
	{
		free(ptr);
	}
private:
	Factory<TerrainNode>* factory;

	Window* window;
	Camera* camera;
	
	TerrainMaps* maps;

	bool leaf;
	int lod;
	Vector2f location;
	Vector3f worldPosition;
	Vector2f index;
	float gap;

	RendererComponent* renderer;
	RendererComponent* wireframeRendererComponent;
	Material* material;

	Transform* localTransform;
};

#endif