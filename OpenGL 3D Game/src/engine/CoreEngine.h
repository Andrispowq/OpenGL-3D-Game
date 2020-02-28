#ifndef COREENGINE_H
#define COREENGINE_H

#include "engine/config/FrameworkConfig.h"
#include "engine/engines/Engine.h"
#include "engine/core/util/Time.h"

#include <chrono>
#include <thread>

class CoreEngine
{
public:
	CoreEngine();
	virtual ~CoreEngine();

	void Start();
private:
	void Stop();

	void Run();

	void Input() { engine->Input(static_cast<float>(frameTime)); }
	void Update() { engine->Update(); }
	void Render() { engine->Render(); }

	CoreEngine(const CoreEngine& engine) = delete;
	CoreEngine operator=(const CoreEngine& engine) = delete;
private:
	bool running;
	double frameTime;

	Engine* engine;
};

#endif