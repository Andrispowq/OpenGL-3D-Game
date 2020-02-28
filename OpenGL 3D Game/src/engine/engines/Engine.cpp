#include "Engine.h"
#include "engine/scene/Scene.h"

Engine::Engine()
{
	frameTime = 0;

	//Config loading
	FrameworkConfig::LoadConfig("res/config/framework.cfg");

	//Rootobject init
	root = new GameObject();

	//Engines' initialization
	renderingEngine = new RenderingEngine();

	InputInstance.Init(*(renderingEngine->GetWindow()));

	Scene::CreateScene(root);
}

Engine::~Engine()
{
	delete renderingEngine;
	Scene::DeleteData();

	delete root;
}

void Engine::Input(const float frameTime)
{
	this->frameTime = frameTime;

	InputInstance.Update();

	renderingEngine->Input(frameTime);
}

void Engine::Update()
{
	renderingEngine->Update(frameTime);
}

void Engine::Render()
{
	renderingEngine->Render(root);
}