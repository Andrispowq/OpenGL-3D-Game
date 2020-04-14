#include "engine/prehistoric/core/util/Includes.hpp"
#include "Scene.h"

Terrain* Scene::terrain;

void Scene::CreateScene(GameObject* root, Window* window, Camera* camera)
{
	//WorldLoader loader;
	//loader.LoadWorld("res/world/testLevel.wrld", root, window);

	terrain = new Terrain(window, camera);
	terrain->UpdateQuadtree();
}

void Scene::DeleteData()
{
	delete terrain;

	Renderer::CleanUp();
	Renderable::CleanUp();
}