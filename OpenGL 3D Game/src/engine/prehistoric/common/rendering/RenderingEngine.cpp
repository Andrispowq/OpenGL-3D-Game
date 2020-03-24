#include "engine/prehistoric/core/util/Includes.hpp"
#include "RenderingEngine.h"
#include "engine/prehistoric/component/renderer/Renderable.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/common/util/DeviceProperties.h"
#include "engine/platform/vulkan/framework/context/VkContext.h"

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
		system("PAUSE");
		exit(-1);
	}

	if (FrameworkConfig::api == OpenGL)
	{
		DeviceProperties::GetInstance().ListProperties(nullptr);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		DeviceProperties::GetInstance().ListProperties(window->GetContext()->GetPhysicalDevice());
	}


	window->SetClearColor(0.23f, 0.78f, 0.88f, 1.0f);

	camera = new Camera(2.0f, 8.0f, 0.8f, 75.0f, Vector3f(0, 2, -3));
	camera->RotateX(20);
	camera->LogStage();

	camera->SetSpeedControl({ 0, MOUSE_SCROLL, 0 });
}

RenderingEngine::~RenderingEngine()
{
	DeviceProperties::DeleteInstance();

	delete window;
	delete camera;
}

void RenderingEngine::Init() const
{
	CameraInput* keyInput = new CameraInput({ GLFW_KEY_W, KEY_HELD, 0 }, { GLFW_KEY_S, KEY_HELD, 0 }, { GLFW_KEY_D, KEY_HELD, 0 }, { GLFW_KEY_A, KEY_HELD, 0 },
		{ GLFW_KEY_UP, KEY_HELD, 0 }, { GLFW_KEY_DOWN, KEY_HELD, 0 }, { GLFW_KEY_RIGHT, KEY_HELD, 0 }, { GLFW_KEY_LEFT, KEY_HELD, 0 });

	CameraInput* controllerInput = new CameraInput({ GLFW_GAMEPAD_AXIS_LEFT_Y, JOYSTICK_AXIS_MOVED_POSITIVE, 0 }, { GLFW_GAMEPAD_AXIS_LEFT_Y, JOYSTICK_AXIS_MOVED_NEGATIVE, 0 }, { GLFW_GAMEPAD_AXIS_LEFT_X, JOYSTICK_AXIS_MOVED_POSITIVE, 0 }, { GLFW_GAMEPAD_AXIS_LEFT_X, JOYSTICK_AXIS_MOVED_NEGATIVE, 0 },
		{ GLFW_GAMEPAD_AXIS_RIGHT_Y, JOYSTICK_AXIS_MOVED_POSITIVE, 0 }, { GLFW_GAMEPAD_AXIS_RIGHT_Y, JOYSTICK_AXIS_MOVED_NEGATIVE, 0 }, { GLFW_GAMEPAD_AXIS_RIGHT_X, JOYSTICK_AXIS_MOVED_POSITIVE, 0 }, { GLFW_GAMEPAD_AXIS_RIGHT_X, JOYSTICK_AXIS_MOVED_NEGATIVE, 0 });

	camera->AddCameraInput(keyInput);
	camera->AddCameraInput(controllerInput);
}

void RenderingEngine::Input(const float delta)
{
	window->ClearScreen();
	window->Input();
}

void RenderingEngine::Update(const float delta)
{
	if (InputInstance.IsKeyPushed(GLFW_KEY_ESCAPE))
	{
		window->SetClosed(true);
	}

	camera->Input(window, delta);
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
	lights.clear();
}

void RenderingEngine::AddModel(Renderable* renderable)
{
	models.push_back(renderable);
}

void RenderingEngine::AddLight(Light* light)
{
	lights.push_back(light);
}