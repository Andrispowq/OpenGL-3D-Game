#include "engine/prehistoric/core/util/Includes.hpp"
#include "CoreEngine.h"

CoreEngine::CoreEngine()
{
	running = false;
	frameTime = 1 / 120.0;

	engine = new Engine();
}

CoreEngine::~CoreEngine()
{
	delete engine;
}

void CoreEngine::Start()
{
	if (running)
		return;

	running = true;
	Run();
}

void CoreEngine::Stop()
{
	if (!running)
		return;

	running = false;
}

void CoreEngine::Run()
{
	unsigned int frames = 0;
	double frameCounter = 0;

	double lastTime = Time::getTime();
	double unprocessedTime = 0;

	while (running)
	{
		bool render = false;

		double startTime = Time::getTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		while (unprocessedTime > frameTime)
		{
			render = true;

			unprocessedTime -= frameTime;

			if (engine->GetRenderingEngine()->GetWindow()->ShouldClose())
			{
				Stop();
				break;
			}

			Input(passedTime);
			Update();

			if (frameCounter >= 1.0)
			{
				PR_LOG(CYAN, "FPS: %i\n", frames);
				frames = 0;
				frameCounter = 0;
			}
		}

		if (render)
		{
			Render();
			frames++;
		}
		else
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
	}
}