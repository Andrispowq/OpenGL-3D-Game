#include "engine/prehistoric/core/util/Includes.hpp"
#include "RenderingEngine.h"
#include "engine/prehistoric/component/renderer/Renderable.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/common/util/DeviceProperties.h"
#include "engine/platform/vulkan/framework/context/VKContext.h"

#include "engine/prehistoric/component/gui/button/GUIButton.h"

RenderingEngine::RenderingEngine()
{
#if defined(PR_WINDOWS_64)
	window = new WindowsWindow(FrameworkConfig::windowWidth, FrameworkConfig::windowHeight, FrameworkConfig::windowName.c_str(), FrameworkConfig::windowFullScreen);
#elif
	PR_LOG_RUNTIME_ERROR("Unsupported platform is being used!\n");
#endif

	if (!window->Create())
	{
		PR_LOG_RUNTIME_ERROR("The creation of the window has failed!\n");
	}

	Capabilities::GetInstance()->QueryCapabilities(window->GetContext()->GetPhysicalDevice());
	DeviceProperties properties;
	properties.ListProperties(*Capabilities::GetInstance());

	window->SetClearColor(0.23f, 0.78f, 0.88f, 1.0f);

	camera = new Camera(5.0f, 50.0f, 0.8f, 80.0f, Vector3f(0, 2, -3));
	camera->RotateX(20);
	camera->LogStage();

	camera->setSpeedControl({ MOUSE_SCROLL, PR_KEY_UNKNOWN, PR_JOYSTICK_1 });
}

RenderingEngine::~RenderingEngine()
{
	delete window;
	delete camera;
}

void RenderingEngine::Init() const
{
	CameraInput* keyInput = new CameraInput({ KEY_HELD, PR_KEY_W, PR_JOYSTICK_1 }, { KEY_HELD, PR_KEY_S, PR_JOYSTICK_1 }, { KEY_HELD, PR_KEY_D, PR_JOYSTICK_1 }, { KEY_HELD, PR_KEY_A, PR_JOYSTICK_1 },
		{ KEY_HELD, PR_KEY_UP, PR_JOYSTICK_1 }, { KEY_HELD, PR_KEY_DOWN, PR_JOYSTICK_1 }, { KEY_HELD, PR_KEY_RIGHT, PR_JOYSTICK_1 }, { KEY_HELD, PR_KEY_LEFT, PR_JOYSTICK_1 });

	camera->AddCameraInput(keyInput);
}

void RenderingEngine::Input(const float delta)
{
	window->ClearScreen();
	window->Input();
}

bool br = false;

void RenderingEngine::Update(const float delta)
{
	if (InputInstance.IsKeyPushed(PR_KEY_ESCAPE))
	{
		window->SetClosed(true);
	}

	if (InputInstance.IsKeyPushed(PR_KEY_E))
	{
		wireframeMode = !wireframeMode;
	}

	if (window->GetResized())
	{
		Renderable::RecreatePipelines();
		window->SetResized(false);
	}

	if (InputInstance.IsKeyPushed(PR_KEY_Q))
	{
		br = true;
	}

	camera->Input(window, delta);
}

void RenderingEngine::Render(GameObject* root)
{
	root->PreRender(this);

	if (br)
	{
		PR_LOG_MESSAGE("hello\n");
		br = false;
	}

	for (auto pipeline : models)
	{
		pipeline.first->BindPipeline();
		pipeline.first->getShader()->UpdateShaderUniforms(camera, lights);
		pipeline.first->getShader()->UpdateSharedUniforms(pipeline.second[0]->GetParent());

		for (auto renderer : pipeline.second)
		{
			renderer->BatchRender(*this);
		}

		pipeline.first->UnbindPipeline();
	}

	window->Render();
	models.clear();
	lights.clear();
}

static bool FindElement(Pipeline* pipeline, std::unordered_map<Pipeline*, std::vector<Renderable*>> pipelines)
{
	for (const auto element : pipelines)
	{
		if ((*pipeline).operator==(*element.first))
		{
			return true;
		}
	}

	return false;
}

void RenderingEngine::AddModel(Renderable* renderable)
{
	Pipeline* pipeline = renderable->GetPipeline();

	if(FindElement(pipeline, models))
	{
		auto& renderables = models[pipeline];
		renderables.push_back(renderable);
	}
	else
	{
		std::vector<Renderable*> renderers = { renderable };
		models.insert(std::make_pair(pipeline, renderers));
	}
}

void RenderingEngine::AddLight(Light* light)
{
	lights.push_back(light);
}