#include "engine/prehistoric/core/util/Includes.hpp"
#include "Engine.h"

Engine::Engine()
{
	frameTime = 0;

	//Config loading
	FrameworkConfig::LoadConfig("res/config/framework.cfg");
	EngineConfig::LoadConfig("res/config/engine.cfg");

	//Rootobject init
	root = new GameObject();

	//Engines' initialization
	renderingEngine = new RenderingEngine();
	InputInstance.Init(renderingEngine->GetWindow());
	renderingEngine->Init();

	Scene::CreateScene(root);
}

Engine::~Engine()
{
	delete renderingEngine;
	Scene::DeleteData();
	Input::DeleteInstance();

	delete root;
}

void Engine::Input(const float frameTime)
{
	this->frameTime = frameTime;

	InputInstance.Update();

	root->PreInput(frameTime);
	renderingEngine->Input(frameTime);
}

void Engine::Update()
{
	root->PreUpdate(frameTime);
	renderingEngine->Update(frameTime);
}

void Engine::Render()
{
	renderingEngine->Render(root);
}