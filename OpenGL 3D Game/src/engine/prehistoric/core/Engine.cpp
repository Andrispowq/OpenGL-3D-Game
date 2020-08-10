#include "engine/prehistoric/core/util/Includes.hpp"
#include "Engine.h"

Engine::Engine()
	: root(nullptr), renderingEngine(nullptr), audioEngine(nullptr), manager(nullptr)
{
	frameTime = 0;

	//Config loading
	FrameworkConfig::LoadConfig("res/config/framework.cfg");
	EngineConfig::LoadConfig("res/config/engine.cfg");
	AtmosphereConfig::LoadConfig("res/config/atmosphere.cfg");

	//Rootobject init
	root = std::make_unique<GameObject>();

	//Engines' initialization
	renderingEngine = std::make_unique<RenderingEngine>();
	audioEngine = std::make_unique<AudioEngine>();

	InputInstance.Init(renderingEngine->getWindow());
	renderingEngine->Init();

	manager = std::make_unique<AssetManager>(renderingEngine->getWindow());
	
	//Loading configs that depend on some engine feature like the window
	TerrainConfig::LoadConfig("res/config/terrain.cfg", renderingEngine->getWindow(), manager.get());
	Scene::CreateScene(root.get(), renderingEngine->getWindow(), manager.get(), renderingEngine->getCamera());
}

Engine::~Engine()
{
	Scene::DeleteData();
	Input::DeleteInstance();
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
	renderingEngine->getWindow()->ClearScreen();
	root->PreRender(renderingEngine.get());

	renderingEngine->Render();
}