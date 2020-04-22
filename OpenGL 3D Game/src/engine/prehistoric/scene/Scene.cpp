#include "engine/prehistoric/core/util/Includes.hpp"
#include "Scene.h"

#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"
#include "engine/platform/vulkan/buffer/VKMeshVBO.h"
#include "engine/platform/vulkan/texture/VkTexture.h"
#include "engine/platform/vulkan/rendering/shaders/pbr/VKPBRShader.h"

Terrain* Scene::terrain;

void Scene::CreateScene(GameObject* root, Window* window, Camera* camera)
{
	WorldLoader loader;
	loader.LoadWorld("res/world/testLevel.wrld", root, window);

	/*VKMeshVBO* vbo = (VKMeshVBO*)OBJLoader::LoadModel("res/models/", "quad.obj", "", window);

	VKShader* shader = new VKPBRShader(window);
	VKPipeline* pipeline = new VKPipeline(shader, vbo);

	pipeline->SetBackfaceCulling(true);

	pipeline->SetViewportStart({ 0, 0 });
	pipeline->SetViewportSize({ (float) FrameworkConfig::windowWidth, (float) FrameworkConfig::windowHeight });
	pipeline->SetScissorStart({ 0, 0 });
	pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });
	
	pipeline->CreatePipeline(window);

	Material* material = new Material(window);
	material->AddTexture(ALBEDO_MAP, TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_DIF.png", window));
	material->AddTexture(NORMAL_MAP, TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_NRM.png", window));
	material->AddTexture(METALLIC_MAP, TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_MET.png", window));
	material->AddTexture(ROUGHNESS_MAP, TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_RGH.png", window));

	//material->AddFloat(METALLIC, new float(1.0));
	//material->AddFloat(ROUGHNESS, new float(0.1));
	material->AddFloat(OCCLUSION, new float(01));

	Renderer* renderer = new Renderer(pipeline, material, window);

	GameObject* obj = new GameObject();
	obj->AddComponent(RENDERER_COMPONENT, renderer);
	obj->Rotate({ -90, 0, 0 });
	obj->Move({ 0, 3, 0 });
	root->AddChild("VK", obj);

	terrain = new Terrain(window, camera);
	terrain->UpdateQuadtree();

	root->AddChild("Terrain", terrain);

	GameObject* light = new GameObject();
	light->AddComponent("light", new Light(Vector3f(1), Vector3f(100)));
	light->Move({ 0, 30, 0 });
	root->AddChild("l", light);

	GameObject* light2 = new GameObject();
	light2->AddComponent("light", new Light(Vector3f(1, 0, 0), Vector3f(10000)));
	light2->Move({ -20, 20, 40 });
	root->AddChild("l2", light2);*/
}

void Scene::DeleteData()
{
	Renderer::CleanUp();
	Renderable::CleanUp();
}