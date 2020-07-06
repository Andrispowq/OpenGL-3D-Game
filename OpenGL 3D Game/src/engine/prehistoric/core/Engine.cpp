#include "engine/prehistoric/core/util/Includes.hpp"
#include "Engine.h"

Engine::Engine()
{
	frameTime = 0;

	//Config loading
	FrameworkConfig::LoadConfig("res/config/framework.cfg");
	EngineConfig::LoadConfig("res/config/engine.cfg");
	AtmosphereConfig::LoadConfig("res/config/atmosphere.cfg");

	//Rootobject init
	root = new GameObject();

	//Engines' initialization
	renderingEngine = new RenderingEngine();
	audioEngine = new AudioEngine();

	InputInstance.Init(renderingEngine->GetWindow());
	renderingEngine->Init();
	
	//Loading configs that depend on some engine feature like the window
	TerrainConfig::LoadConfig("res/config/terrain.cfg", renderingEngine->GetWindow());
	Scene::CreateScene(root, renderingEngine->GetWindow(), renderingEngine->GetCamera());
}

Engine::~Engine()
{
	Scene::DeleteData();
	Input::DeleteInstance();

	delete renderingEngine;
	delete audioEngine;

	delete root;
}

void Engine::Input(float frameTime)
{
	this->frameTime = frameTime;

	InputInstance.Update();

	renderingEngine->Input();
}

//For consistency: Engine updates the root node, and then the engines do the work they need to
//Most components will register themselves into an engine's list of components, and then the engine can update them
void Engine::Update()
{
	root->PreUpdate(this);

	audioEngine->Update(frameTime);
	renderingEngine->Update(frameTime);
}

void Engine::Render()
{
	//We cannot detect keys in the render loop, because it runs differently then the update and input loop
	renderingEngine->GetWindow()->ClearScreen();
	root->PreRender(renderingEngine);

	renderingEngine->Render(root);
}