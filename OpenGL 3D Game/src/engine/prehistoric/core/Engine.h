#ifndef ENGINE_H
#define ENGINE_H

#include "engine/prehistoric/common/rendering/RenderingEngine.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"

class Engine
{
public:
	Engine();
	virtual ~Engine();

	void Input(const float frameTime);
	void Update();
	void Render();

	inline RenderingEngine* GetRenderingEngine() const { return renderingEngine; }

	Engine(const Engine& engine) = delete;
	Engine operator=(const Engine& engine) = delete;
private:
	//Root object
	GameObject* root;

	//Engines
	RenderingEngine* renderingEngine;

	//Frametime, set once per update
	float frameTime;
};

#endif