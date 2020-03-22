#include "engine/prehistoric/core/util/Includes.hpp"
#include "Scene.h"

MeshVBO* Scene::vbo = nullptr;
Shader* Scene::shader = nullptr;

void Scene::CreateScene(GameObject* root)
{
	WorldLoader loader;
	//loader.LoadWorld("res/world/testLevel.wrld", root);

	/*vbo = ModelFabricator::CreateQuad();

	if (FrameworkConfig::api == OpenGL)
	{
		shader = new GLPBRShader();
	}

	Material* matN = new Material;
	matN->AddTexture("albedoMap", TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_DIF.png"));
	matN->AddTexture("normalMap", TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_NRM.png"));
	matN->AddTexture("roughnessMap", TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_RGH.png"));
	matN->AddTexture("metallicMap", TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_MET.png"));

	Material* mat = new Material;
	mat->AddTexture("albedoMap", TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_DIF.png"));
	mat->AddTexture("roughnessMap", TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_RGH.png"));
	mat->AddTexture("metallicMap", TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_MET.png"));

	GameObject* child = new GameObject();
	child->AddComponent("renderer", new Renderer(vbo, shader, matN));
	child->SetRotation({90, 0, 0});

	GameObject* child2 = new GameObject();
	child2->AddComponent("renderer", new Renderer(vbo, shader, mat));
	child2->Move({ -2, 0, 0 });
	child2->SetRotation({ 90, 0, 0 });

	root->AddChild("plane-normalmap", child);
	root->AddChild("plane", child2);

	GameObject* light = new GameObject;
	light->AddComponent("light", new Light({ 1 }, { 100 }));
	light->Move({0, 4, 2});

	root->AddChild(light);*/
}

void Scene::DeleteData()
{
	Renderer::CleanUp();
	Renderable::CleanUp();
}