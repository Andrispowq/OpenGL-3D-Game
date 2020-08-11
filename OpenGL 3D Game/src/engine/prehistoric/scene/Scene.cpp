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

#include "engine/prehistoric/resources/AssembledAssetManager.h"

//We go around in a circle, from -2000 to 2000 on the y and z axes
static void sun_move_function(GameObject* object, float frameTime)
{
	constexpr float anglesPerSecond = 20.0f;
	Vector3f old = object->getWorldTransform().getPosition();
	old = old.rotate(Vector3f(anglesPerSecond, 0, 0) * frameTime);

	object->SetPosition(old);
}

Scene::Scene(GameObject* root, Window* window, AssembledAssetManager* manager, Camera* camera)
{
	WorldLoader loader;
	loader.LoadWorld("res/world/testLevel.wrld", root, window, manager);

	if (FrameworkConfig::api == Vulkan)
	{
		size_t vboID = manager->getAssetManager()->getResource<VertexBuffer>("quad.obj");
		size_t shaderID = manager->getAssetManager()->getResource<Shader>("pbr");

		size_t pipelineID = manager->loadResource<Pipeline>(new VKGraphicsPipeline(window, manager->getAssetManager(), shaderID, vboID));

		Material* material = new Material(manager->getAssetManager(), window);
		material->addTexture(ALBEDO_MAP, manager->getAssetManager()->getResource<Texture>("oakFloor_DIF.png"));
		material->addTexture(NORMAL_MAP, manager->getAssetManager()->getResource<Texture>("oakFloor_NRM.png"));
		material->addTexture(METALLIC_MAP, manager->getAssetManager()->getResource<Texture>("oakFloor_MET.png"));
		material->addTexture(ROUGHNESS_MAP, manager->getAssetManager()->getResource<Texture>("oakFloor_RGH.png"));

		size_t materialID = manager->loadResource<Material>(material);

		//material->AddFloat(METALLIC, 1.0);
		//material->AddFloat(ROUGHNESS, 0.3);
		material->addFloat(OCCLUSION, 1);

		RendererComponent* renderer = new RendererComponent(pipelineID, materialID, window, manager);

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
		root->AddChild("Atmosphere", new Atmosphere(window, manager));		

		Terrain* terrain = new Terrain(window, manager, camera);
		terrain->UpdateQuadtree();

		root->AddChild("Terrain", terrain);

		GameObject* slider = new GUISlider(window, manager, 0.0f, 2.0f, terrain->getMaps()->getHeightmap(), &EngineConfig::rendererExposure, sizeof(float), true);
		slider->SetPosition({ 0.5f, 0.5f, 0 });
		slider->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider", slider);

		GameObject* slider2 = new GUISlider(window, manager, 1.0f, 3.4f, terrain->getMaps()->getHeightmap(), &EngineConfig::rendererGamma, sizeof(float), true);
		slider2->SetPosition({ 0.5f, 0.25f, 0 });
		slider2->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider2", slider2);

		/*GameObject* slider3 = new GUISlider(window, manager, 2000.0f, 14000.0f, terrain->getMaps()->getHeightmap(), &AtmosphereConfig::rayleighHeightScale, sizeof(float), true);
		slider3->SetPosition({ -0.5f, 0.5f, 0 });
		slider3->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider3", slider3);

		GameObject* slider4 = new GUISlider(window, manager, 0.000021f / 2.0f , 0.000021f * 2.0f, terrain->getMaps()->getHeightmap(), &AtmosphereConfig::mie, sizeof(float), true);
		slider4->SetPosition({ -0.5f, 0.25f, 0 });
		slider4->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider4", slider4);

		GameObject* slider5 = new GUISlider(window, manager, 200.0f, 2200.0f, terrain->getMaps()->getHeightmap(), &AtmosphereConfig::mieHeightScale, sizeof(float), true);
		slider5->SetPosition({ -0.5f, 0.0f, 0 });
		slider5->SetScale({ 0.125f, 0.05f, 1 });
		root->AddChild("slider5", slider5);*/

		GameObject* sun = new GameObject();
		sun->setUpdateFunction(sun_move_function);
		sun->AddComponent(LIGHT_COMPONENT, new Light(Vector3f(1, 1, 1), Vector3f(200000000), true));
		sun->SetPosition({ -2000, 0, 0 });
		root->AddChild("sun", sun);

		GameObject* light = new GameObject();
		light->AddComponent(LIGHT_COMPONENT, new Light(Vector3f(1, 0.56f, 0.34f), Vector3f(300000)));
		light->Move({ 100, 200, -300 });
		root->AddChild("light", light);
	}
}