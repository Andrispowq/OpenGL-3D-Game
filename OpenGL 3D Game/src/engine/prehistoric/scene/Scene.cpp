#include "engine/prehistoric/core/util/Includes.hpp"
#include "Scene.h"

#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"
#include "engine/platform/vulkan/buffer/VKMeshVBO.h"
#include "engine/platform/vulkan/texture/VkTexture.h"
#include "engine/platform/vulkan/rendering/shaders/pbr/VKPBRShader.h"

#include "engine/prehistoric/modules/gui/GUIElement.h"
#include "engine/prehistoric/modules/gui/button/GUIButton.h"

#include "engine/prehistoric/modules/atmosphere/Atmosphere.h"

Terrain* Scene::terrain;

static void sun_move_function(GameObject* object, float frameTime)
{
	object->Move(Vector3f(0, 40, 0) * frameTime);
}

void Scene::CreateScene(GameObject* root, Window* window, Camera* camera)
{
	WorldLoader loader;
	loader.LoadWorld("res/world/testLevel.wrld", root, window);

	if (FrameworkConfig::api == Vulkan)
	{
		VKMeshVBO* vbo = (VKMeshVBO*)OBJLoader::LoadModel("res/models/", "quad.obj", "", window);
		vbo->SetFrontFace(FrontFace::CLOCKWISE);

		VKShader* shader = new VKPBRShader(window);

		VKGraphicsPipeline* pipeline = new VKGraphicsPipeline(shader, vbo);

		pipeline->SetBackfaceCulling(false);

		pipeline->SetViewportStart({ 0, 0 });
		pipeline->SetViewportSize({ (float)FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight });
		pipeline->SetScissorStart({ 0, 0 });
		pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

		pipeline->CreatePipeline(window);

		Material* material = new Material(window);
		material->AddTexture(ALBEDO_MAP, TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_DIF.png", window));
		material->AddTexture(NORMAL_MAP, TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_NRM.png", window));
		material->AddTexture(METALLIC_MAP, TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_MET.png", window));
		material->AddTexture(ROUGHNESS_MAP, TextureLoader::LoadTexture("res/textures/oakFloor/oakFloor_RGH.png", window));

		//material->AddFloat(METALLIC, 1.0);
		//material->AddFloat(ROUGHNESS, 0.3);
		material->AddFloat(OCCLUSION, 1);

		Renderer* renderer = new Renderer(pipeline, material, window);

		GameObject* obj = new GameObject();
		obj->AddComponent(RENDERER_COMPONENT, renderer);
		obj->Rotate({ 0, 0, 0 });
		obj->Move({ 0, 0, -5 });
		obj->SetScale({ 0.5f, 0.5f, 0.5f });
		root->AddChild("OBJ", obj);

		GameObject* light2 = new GameObject();
		light2->AddComponent(LIGHT_COMPONENT, new Light(Vector3f(1, 0, 0), Vector3f(1000)));
		light2->Move({ -20, 40, 40 });
		root->AddChild("l", light2);
	}
	else
	{
		root->AddChild("Atmosphere", new Atmosphere(window));		

		terrain = new Terrain(window, camera);
		terrain->UpdateQuadtree();

		//root->AddChild("Terrain", terrain);

		GameObject* button1 = new GUIButton(window, terrain->getMaps()->getSplatmap(), &EngineConfig::rendererExposure, sizeof(float), true);
		button1->SetPosition({ 0.5f, 0.5f, 0 });
		button1->SetScale({ 0.125f, 0.125f, 1 });
		root->AddChild("button1", button1);

		GameObject* sun = new GameObject();
		sun->AddComponent(LIGHT_COMPONENT, new Light(Vector3f(1, 1, 1), Vector3f(200000000), true, sun_move_function));
		sun->SetPosition({ -2000, -1000, 2000 });
		root->AddChild("sun", sun);

		GameObject* light = new GameObject();
		light->AddComponent(LIGHT_COMPONENT, new Light(Vector3f(1, 0.56f, 0.34f), Vector3f(300000)));
		light->Move({ 100, 200, -300 });
		root->AddChild("light", light);
	}
}

void Scene::DeleteData()
{
	Renderer::CleanUp();
	Renderable::CleanUp();
}