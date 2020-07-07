#include "engine/prehistoric/core/util/Includes.hpp"
#include "RenderingEngine.h"
#include "engine/prehistoric/component/renderer/Renderable.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/common/util/DeviceProperties.h"
#include "engine/platform/vulkan/framework/context/VKContext.h"

#include "engine/prehistoric/modules/gui/button/GUIButton.h"

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

	if (FrameworkConfig::api == OpenGL)
	{
		camera = new Camera(5.0f, 50.0f, 0.8f, 80.0f, Vector3f(-178, 102, -47));
		camera->RotateY(-80);
		camera->RotateX(30);
	}
	else
	{
		camera = new Camera(5.0f, 50.0f, 0.8f, 80.0f, Vector3f(0, 0, -2));
	}

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

void RenderingEngine::Input()
{
	window->Input();
}

void RenderingEngine::Update(float delta)
{
	if (InputInstance.IsKeyPushed(PR_KEY_ESCAPE))
	{
		window->SetClosed(true);
	}

	if (InputInstance.IsKeyPushed(PR_KEY_F11))
	{
		//window->SetFullscreen(true);
	}

	if (InputInstance.IsKeyPushed(PR_KEY_E))
	{
		wireframeMode = !wireframeMode;
	}

	if (window->GetResized())
	{
		camera->SetProjection(camera->getFov(), window->GetWidth(), window->GetHeight());
		Renderable::RecreatePipelines();
		window->SetResized(false);
	}

	camera->Update(window, delta);
}

void RenderingEngine::Render(GameObject* root)
{
	window->GetSwapchain()->PrepareRendering();

	for (auto pipeline : models)
	{
		Pipeline* pl = pipeline.first;

		pl->BindPipeline();
		pl->getShader()->UpdateShaderUniforms(camera, lights);
		//The models array is guaranteed to contain at least on entry for each pipeline, so this is safe to get the first element
		pl->getShader()->UpdateSharedUniforms(pipeline.second[0]->GetParent()); 

		for (auto renderer : pipeline.second)
		{
			renderer->BatchRender(*this);
		}

		pl->UnbindPipeline();
	}

	window->GetSwapchain()->EndRendering();

	window->Render();
	//We clean up this data before rendering and not after rendering so that it can be used in the PreUpdate functions of the Atmosphere for examples
	models.clear();
	lights.clear();
}

void RenderingEngine::AddModel(Renderable* renderable)
{
	Pipeline* pipeline = renderable->GetPipeline();

	if(models.find(pipeline) != models.end())
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
	if (light->IsSun())
		sun = light;

	lights.push_back(light);
}