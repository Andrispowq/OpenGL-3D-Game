#ifndef SCENE_H
#define SCENE_H

#include "engine/prehistoric/component/renderer/RendererComponent.h"
#include "engine/prehistoric/world/WorldLoader.h"

#include "engine/prehistoric/modules/terrain/Terrain.h"

class Scene
{
public:
	static void CreateScene(GameObject* root, Window* window, AssetManager* manager, Camera* camera);
	static void DeleteData();
private:
	Scene() {}
	virtual ~Scene() {}
};

#endif