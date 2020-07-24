#include "engine/prehistoric/core/util/Includes.hpp"
#include "Scene.h"

#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"
#include "engine/platform/vulkan/buffer/VKMeshVertexBuffer.h"
#include "engine/platform/vulkan/texture/VkTexture.h"
#include "engine/platform/vulkan/rendering/shaders/pbr/VKPBRShader.h"

#include "engine/prehistoric/modules/gui/GUIElement.h"
#include "engine/prehistoric/modules/gui/button/GUIButton.h"
#include "engine/prehistoric/modules/gui/slider/GUISlider.h"

#include "engine/prehistoric/modules/atmosphere/Atmosphere.h"

static void sun_move_function(GameObject* object, float frameTime)
{
	object->Move(Vector3f(0, 40, 0) * frameTime);
}

void Scene::CreateScene(GameObject* root, Window* window, Camera* camera)
{
	WorldLoader loader;
	//loader.LoadWorld("res/world/testLevel.wrld", root, window);

	if (FrameworkConfig::api == Vulkan)
	{
		VKMeshVertexBuffer* vbo = (VKMeshVertexBuffer*)OBJLoader::LoadModel("res/models/", "quad.obj", "", window);
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

		Terrain* terrain = new Terrain(window, camera);
		terrain->UpdateQuadtree();

		root->AddChild("Terrain", terrain);

		GameObject* slider = new GUISlider(window, 0.0f, 2.0f, terrain->getMaps()->getHeightmap(), &EngineConfig::rendererExposure, sizeof(float), true);
		slider->SetPosition({ 0.5f, 0.5f, 0 });
		slider->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider", slider);

		GameObject* slider2 = new GUISlider(window, 1.0f, 3.4f, terrain->getMaps()->getHeightmap(), &EngineConfig::rendererGamma, sizeof(float), true);
		slider2->SetPosition({ 0.5f, 0.25f, 0 });
		slider2->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider2", slider2);

		/*GameObject* slider3 = new GUISlider(window, 2000.0f, 14000.0f, terrain->getMaps()->getHeightmap(), &AtmosphereConfig::rayleighHeightScale, sizeof(float), true);
		slider3->SetPosition({ -0.5f, 0.5f, 0 });
		slider3->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider3", slider3);

		GameObject* slider4 = new GUISlider(window, 0.000021f / 2.0f , 0.000021f * 2.0f, terrain->getMaps()->getHeightmap(), &AtmosphereConfig::mie, sizeof(float), true);
		slider4->SetPosition({ -0.5f, 0.25f, 0 });
		slider4->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider4", slider4);

		GameObject* slider5 = new GUISlider(window, 200.0f, 2200.0f, terrain->getMaps()->getHeightmap(), &AtmosphereConfig::mieHeightScale, sizeof(float), true);
		slider5->SetPosition({ -0.5f, 0.0f, 0 });
		slider5->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider5", slider5);*/

		GameObject* sun = new GameObject();
		sun->AddComponent(LIGHT_COMPONENT, new Light(Vector3f(1, 1, 1), Vector3f(200000000), true, sun_move_function));
		sun->SetPosition({ -2000, -2000, 0 });
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
}