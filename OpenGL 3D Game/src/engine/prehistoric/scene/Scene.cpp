#include "engine/prehistoric/core/util/Includes.hpp"
#include "Scene.h"
#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"
#include "engine/platform/vulkan/rendering/shaders/VkShader.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"
#include "engine/platform/vulkan/buffer/VKMeshVBO.h"
#include "engine/platform/vulkan/framework/context/VKContext.h"

MeshVBO* Scene::vbo = nullptr;
Pipeline* Scene::pipeline = nullptr;

void Scene::CreateScene(GameObject* root, Window* window)
{
	WorldLoader loader;
	
	if (FrameworkConfig::api == OpenGL)
	{
		loader.LoadWorld("res/world/testLevel.wrld", root, window);
	}
	else
	{
		vbo = new VKMeshVBO(window->GetContext()->GetPhysicalDevice(), window->GetContext()->GetDevice());

		Mesh mesh;

		mesh.AddVertex(Vertex({ -0.5f, -0.5f, 0.0f }, { 1.0, 0.0 }, -1, { 1.0f, 0.0f, 0.0f }));
		mesh.AddVertex(Vertex({ -0.5f,  0.5f, 0.0f }, { 0.0, 0.0 }, -1, { 0.0f, 1.0f, 0.0f }));
		mesh.AddVertex(Vertex({  0.5f,  0.5f, 0.0f }, { 1.0, 0.0 }, -1, { 0.0f, 0.0f, 1.0f }));
		mesh.AddVertex(Vertex({  0.5f, -0.5f, 0.0f }, { 0.0, 1.0 }, -1, { 1.0f, 1.0f, 1.0f }));

		mesh.AddIndex(0);
		mesh.AddIndex(1);
		mesh.AddIndex(2);
		mesh.AddIndex(2);
		mesh.AddIndex(3);
		mesh.AddIndex(0);

		vbo->Store(mesh);

		Shader* shader = new VKShader(window->GetContext(), window->GetSwapchain());

		shader->AddShader(ResourceLoader::LoadShaderVK("vulkan/basic_VS.spv"), VERTEX_SHADER);
		shader->AddShader(ResourceLoader::LoadShaderVK("vulkan/basic_FS.spv"), FRAGMENT_SHADER);
		shader->CompileShader();

		shader->AddUniform("ubo1.model", VERTEX_SHADER, 0, 0, 16 * sizeof(float));
		shader->AddUniform("ubo2.view", VERTEX_SHADER , 1, 0, 16 * sizeof(float));
		shader->AddUniform("ubo3.proj", VERTEX_SHADER, 2, 0, 16 * sizeof(float));

		pipeline = new VKPipeline(shader);
		pipeline->SetViewportStart({ 0, 0 });
		pipeline->SetViewportSize({ (float)FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight });
		pipeline->SetScissorStart({ 0, 0 });
		pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

		pipeline->CreatePipeline(window, vbo);

		GameObject* obj = new GameObject();
		obj->AddComponent("Renderer", new Renderer(vbo, pipeline, new Material()));
		obj->Move({ 0, 0, 4 });

		root->AddChild("VKOBJ", obj);
	}

	GameObject* light = new GameObject;
	light->AddComponent("light", new Light({ 1 }, { 100 }));
	light->Move({0, 4, 2});

	root->AddChild("Light", light);
}

void Scene::DeleteData()
{
	Renderer::CleanUp();
	Renderable::CleanUp();
}