#ifndef TERRAIN_QUADTREE_H
#define TERRAIN_QUADTREE_H

#include "engine/prehistoric/core/gameObject/Node.h"

#include "TerrainNode.h"

class TerrainQuadtree : public Node
{
public:
	TerrainQuadtree(TerrainMaps& maps, Window* window, Camera* camera);
	virtual ~TerrainQuadtree();

	void UpdateQuadtree();
	std::vector<Vector2f> GeneratePatch();
public:
	static int rootNodes;
private:
	TerrainMaps* maps;
	Window* window;
	Camera* camera;

	static std::vector<Vector2f> vertices;
};

#endif