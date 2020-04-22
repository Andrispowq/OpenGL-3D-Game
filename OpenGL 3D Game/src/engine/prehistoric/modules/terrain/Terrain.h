#ifndef TERRAIN_H
#define TERRAIN_H

#include "TerrainQuadtree.h"

class Terrain : public GameObject
{
public:
	Terrain(Window* window, Camera* camera);
	virtual ~Terrain();

	void UpdateQuadtree();

	virtual void PreRender(RenderingEngine* renderingEngine) override;
private:
	TerrainMaps* maps;
	Window* window;
	Camera* camera;
};

#endif