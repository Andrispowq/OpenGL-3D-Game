#ifndef SCENE_H
#define SCENE_H

#include "engine/prehistoric/component/renderer/Renderer.h"
#include "engine/prehistoric/world/WorldLoader.h"

class Scene
{
public:
	static void CreateScene(GameObject* root);
	static void DeleteData();
private:
	Scene() {}
	virtual ~Scene() {}
private:
	static MeshVBO* vbo;
	static Shader* shader;
};

#endif