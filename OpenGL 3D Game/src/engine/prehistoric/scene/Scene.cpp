#include "engine/prehistoric/core/util/Includes.hpp"
#include "Scene.h"

#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"
#include "engine/platform/vulkan/buffer/VKMeshVBO.h"
#include "engine/platform/vulkan/texture/VkTexture.h"
#include "engine/platform/vulkan/rendering/shaders/pbr/VKPBRShader.h"

#include "engine/prehistoric/component/gui/GUIElement.h"
#include "engine/prehistoric/component/gui/button/GUIButton.h"

Terrain* Scene::terrain;

void Scene::CreateScene(GameObject* root, Window* window, Camera* camera)
{
	WorldLoader loader;
	loader.LoadWorld("res/world/testLevel.wrld", root, window);

	if (FrameworkConfig::api == Vulkan)
	{
		VKMeshVBO* vbo = (VKMeshVBO*)OBJLoader::LoadModel("res/models/", "quad.obj", "", window);

		VKShader* shader = new VKPBRShader(window);
		VKGraphicsPipeline* pipeline = new VKGraphicsPipeline(shader, vbo);

		pipeline->SetBackfaceCulling(true);
		pipeline->SetFrontFace(FrontFace::CLOCKWISE);

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

		//material->AddFloat(METALLIC, new float(1.0));
		//material->AddFloat(ROUGHNESS, new float(0.3));
		material->AddFloat(OCCLUSION, new float(1));

		Renderer* renderer = new Renderer(pipeline, material, window);

		GameObject* obj = new GameObject();
		obj->AddComponent(RENDERER_COMPONENT, renderer);
		obj->Rotate({ 0, 0, 0 });
		obj->Move({ 0, 0, 0 });
		obj->SetScale({ 0.5f, 0.5f, 0.5f });
		root->AddChild("OBJ", obj);

		GameObject* light2 = new GameObject();
		light2->AddComponent("light", new Light(Vector3f(1, 0, 0), Vector3f(1000)));
		light2->Move({ -20, 40, 40 });
		root->AddChild("l", light2);
	}
	else
	{
		terrain = new Terrain(window, camera);
		terrain->UpdateQuadtree();

		root->AddChild("Terrain", terrain);

		GameObject* gui = new GameObject;
		gui->AddComponent("GUI", new GUIElement(window, terrain->getMaps()->getNormalmap()));
		gui->Move({ 0.5f, 0.5f, 0.0f });
		gui->SetScale({ 0.25f, 0.25f, 1.0f });
		//((GUIElement*)gui->GetComponent("GUI"))->setVisible(false);

		root->AddChild("GUI", gui);

		GameObject* sun = new GameObject();
		sun->AddComponent("light", new Light(Vector3f(1, 1, 1), Vector3f(1)));
		sun->Move({ 0, 500, 0 });
		root->AddChild("sun", sun);
	}
}

void Scene::DeleteData()
{
	Renderer::CleanUp();
	Renderable::CleanUp();
}