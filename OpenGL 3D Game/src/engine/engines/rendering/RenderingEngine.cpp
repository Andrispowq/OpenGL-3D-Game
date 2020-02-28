#include "RenderingEngine.h"
#include "engine/component/renderer/Renderable.h"
#include "engine/core/gameObject/GameObject.h"

RenderingEngine::RenderingEngine()
{
	window = new Window(FrameworkConfig::windowWidth, FrameworkConfig::windowHeight, (const char*)FrameworkConfig::windowName.c_str(), FrameworkConfig::windowFullScreen);

	if (!window->create())
	{
		exit(-1);
	}

	window->clearColor(0, 0, 0, 1);

	camera = new Camera(Vector3f(0, 0, 0), 90);
	camera->LogStage();
}

RenderingEngine::~RenderingEngine()
{
	delete window;
	delete camera;
}

void RenderingEngine::Init() const
{
	glFrontFace(GL_CW);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_TEXTURE_2D);
}

void RenderingEngine::Input(const float delta)
{
	window->clearScreen();
	window->Input();
}

void RenderingEngine::Update(const float delta)
{
	camera->Input(*window);
}

void RenderingEngine::Render(GameObject* root)
{
	root->PreRender(this);

	for (Renderable* model : models)
	{
		model->Render(*this);
	}

	window->Render();
	models.clear();
}

void RenderingEngine::AddModel(Renderable* renderable)
{
	models.push_back(renderable);
}