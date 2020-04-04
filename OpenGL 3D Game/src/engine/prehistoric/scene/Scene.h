#ifndef SCENE_H
#define SCENE_H

#include "engine/prehistoric/component/renderer/Renderer.h"
#include "engine/prehistoric/world/WorldLoader.h"

class Scene
{
public:
	static void CreateScene(GameObject* root, Window* window);
	static void DeleteData();
private:
	Scene() {}
	virtual ~Scene() {}
private:
	static MeshVBO* vbo;
	static Pipeline* pipeline;
};

#endif