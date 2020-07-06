#ifndef ENGINE_H
#define ENGINE_H

#include "engine/prehistoric/scene/Scene.h"

#include "engine/prehistoric/engines/RenderingEngine.h"
#include "engine/prehistoric/engines/AudioEngine.h"

#include "engine/prehistoric/core/gameObject/GameObject.h"

#include "engine/config/EngineConfig.h"
#include "engine/config/FrameworkConfig.h"
#include "engine/config/TerrainConfig.h"
#include "engine/config/AtmosphereConfig.h"

class Engine
{
public:
	Engine();
	virtual ~Engine();

	void Input(float frameTime);
	void Update();
	void Render();

	inline RenderingEngine* GetRenderingEngine() const { return renderingEngine; }

	inline float getFrameTime() const { return frameTime; }

	Engine(const Engine& engine) = delete;
	Engine operator=(const Engine& engine) = delete;
	Engine(const Engine&& engine) = delete;
	Engine operator=(const Engine&& engine) = delete;
private:
	//Root object
	GameObject* root;

	//Engines
	RenderingEngine* renderingEngine;
	AudioEngine* audioEngine;

	//Frametime, set once per update
	float frameTime;
};

#endif