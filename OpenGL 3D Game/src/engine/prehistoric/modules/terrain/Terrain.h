#ifndef TERRAIN_H
#define TERRAIN_H

#include "engine/prehistoric/core/gameObject/Node.h"

#include "TerrainQuadtree.h"

class Terrain : public Node
{
public:
	Terrain(Window* window, AssembledAssetManager* manager, Camera* camera);
	virtual ~Terrain() {}

	void PreRender(Renderer* renderer) override;
	void UpdateQuadtree();

	TerrainMaps* getMaps() const { return maps.get(); }
private:
	Window* window;
	Camera* camera;

	std::unique_ptr<TerrainMaps> maps;
};

#endif