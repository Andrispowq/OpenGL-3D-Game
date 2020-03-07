#include "RenderingEngine.h"
#include "engine/prehistoric/component/renderer/Renderable.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/platform/windows/opengl/framework/GLWindow.h"
#include "engine/platform/windows/vulkan/framework/VkWindow.h"

RenderingEngine::RenderingEngine()
{
	if (FrameworkConfig::api == OpenGL)
	{
		window = new GLWindow(FrameworkConfig::windowWidth, FrameworkConfig::windowHeight, (const char*)FrameworkConfig::windowName.c_str(), FrameworkConfig::windowFullScreen);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		window = new VkWindow(FrameworkConfig::windowWidth, FrameworkConfig::windowHeight, (const char*)FrameworkConfig::windowName.c_str(), FrameworkConfig::windowFullScreen);
	}
	
	if (!window->Create())
	{
		exit(-1);
	}

	window->ClearColor(0, 0, 0, 1);

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
	CameraInput* keyInput = new CameraInput({ GLFW_KEY_W, KEY_HELD, 0 }, { GLFW_KEY_S, KEY_HELD, 0 }, { GLFW_KEY_D, KEY_HELD, 0 }, { GLFW_KEY_A, KEY_HELD, 0 },
		{ GLFW_KEY_UP, KEY_HELD, 0 }, { GLFW_KEY_DOWN, KEY_HELD, 0 }, { GLFW_KEY_RIGHT, KEY_HELD, 0 }, { GLFW_KEY_LEFT, KEY_HELD, 0 });

	CameraInput* controllerInput = new CameraInput({ GLFW_GAMEPAD_AXIS_LEFT_Y, JOYSTICK_AXIS_MOVED, 0 }, { GLFW_GAMEPAD_AXIS_LEFT_Y, JOYSTICK_AXIS_MOVED, 0 }, { GLFW_GAMEPAD_AXIS_LEFT_X, JOYSTICK_AXIS_MOVED, 0 }, { GLFW_GAMEPAD_AXIS_LEFT_X, JOYSTICK_AXIS_MOVED, 0 },
		{ GLFW_GAMEPAD_AXIS_RIGHT_Y, JOYSTICK_AXIS_MOVED, 0 }, { GLFW_GAMEPAD_AXIS_RIGHT_Y, JOYSTICK_AXIS_MOVED, 0 }, { GLFW_GAMEPAD_AXIS_RIGHT_X, JOYSTICK_AXIS_MOVED, 0 }, { GLFW_GAMEPAD_AXIS_RIGHT_X, JOYSTICK_AXIS_MOVED, 0 });

	camera->AddCameraInput(keyInput);
	camera->AddCameraInput(controllerInput);

	glFrontFace(GL_CW);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_TEXTURE_2D);
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

	camera->Input(window);
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