#ifndef TERRAIN_H
#define TERRAIN_H

#include "engine/prehistoric/core/gameObject/Node.h"

#include "TerrainQuadtree.h"

class Terrain : public Node
{
public:
	Terrain(Window* window, AssetManager* manager, Camera* camera);
	virtual ~Terrain();

	void PreRender(RenderingEngine* renderingEngine) override;
	void UpdateQuadtree();

	TerrainMaps* getMaps() const { return maps; }
private:
	Window* window;
	Camera* camera;

	TerrainMaps* maps;
};

#endif