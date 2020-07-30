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

	Capabilities::getInstance()->QueryCapabilities(window->getContext()->getPhysicalDevice());
	DeviceProperties properties;
	properties.ListProperties(*Capabilities::getInstance());

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
		window->setClosed(true);
	}

	if (InputInstance.IsKeyPushed(PR_KEY_F11))
	{
		//window->SetFullscreen(true);
	}

	if (InputInstance.IsKeyPushed(PR_KEY_E))
	{
		wireframeMode = !wireframeMode;
	}

	if (window->getResized())
	{
		camera->SetProjection(camera->getFov(), (float)window->getWidth(), (float)window->getHeight());
		Renderable::RecreatePipelines();
		window->setResized(false);
	}

	camera->Update(window, delta);
}

void RenderingEngine::Render(GameObject* root)
{
	window->getSwapchain()->PrepareRendering();

	for (auto pipeline : models_3d)
	{
		Pipeline* pl = pipeline.first;

		pl->BindPipeline();
		pl->getShader()->UpdateShaderUniforms(camera, lights);
		//The models array is guaranteed to contain at least on entry for each pipeline, so this is safe to get the first element
		pl->getShader()->UpdateSharedUniforms(pipeline.second[0]->getParent()); 

		for (auto renderer : pipeline.second)
		{
			renderer->BatchRender(*this);
		}

		pl->UnbindPipeline();
	}

	//TODO: enable alpha blending
	for (auto pipeline : models_transparency)
	{
		Pipeline* pl = pipeline.first;

		pl->BindPipeline();
		pl->getShader()->UpdateShaderUniforms(camera, lights);
		//The models array is guaranteed to contain at least on entry for each pipeline, so this is safe to get the first element
		pl->getShader()->UpdateSharedUniforms(pipeline.second[0]->getParent());

		for (auto renderer : pipeline.second)
		{
			renderer->BatchRender(*this);
		}

		pl->UnbindPipeline();
	}
	//TODO: disable alpha blending and depth testing
	for (auto pipeline : models_2d)
	{
		Pipeline* pl = pipeline.first;

		pl->BindPipeline();
		pl->getShader()->UpdateShaderUniforms(camera, lights);
		//The models array is guaranteed to contain at least on entry for each pipeline, so this is safe to get the first element
		pl->getShader()->UpdateSharedUniforms(pipeline.second[0]->getParent());

		for (auto renderer : pipeline.second)
		{
			renderer->BatchRender(*this);
		}

		pl->UnbindPipeline();
	}
	//TODO: enable depth testing 

	window->getSwapchain()->EndRendering();

	window->Render();

	//Clear the lists for the next iteration
	models_3d.clear();
	models_transparency.clear();
	models_2d.clear();
	lights.clear();
}

void register_model(std::unordered_map<Pipeline*, std::vector<Renderable*>>& map, Renderable* renderable)
{
	Pipeline* pipeline = renderable->getPipeline();

	if (map.find(pipeline) != map.end())
	{
		auto& renderables = map[pipeline];
		renderables.push_back(renderable);
	}
	else
	{
		std::vector<Renderable*> renderers = { renderable };
		map.insert(std::make_pair(pipeline, renderers));
	}
}

void RenderingEngine::AddModel(Renderable* renderable)
{
	switch (renderable->getPriority())
	{
	case RenderPriority::_3D:
		register_model(models_3d, renderable);
		break;
	case RenderPriority::_TRANSPARENCY:
		register_model(models_transparency, renderable);
		break;
	case RenderPriority::_2D:
		register_model(models_2d, renderable);
		break;
	default:
		break;
	}	
}

void RenderingEngine::AddLight(Light* light)
{
	if (light->isSun())
		sun = light;

	lights.push_back(light);
}