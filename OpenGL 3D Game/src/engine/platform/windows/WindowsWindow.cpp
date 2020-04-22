#include "engine/prehistoric/core/util/Includes.hpp"
#include "WindowsWindow.h"

static void error_callback(int error, const char* description)
{
	PR_LOG_ERROR("Error: %s\n", description);
}

bool WindowsWindow::Create()
{
	//Initialisation of GLFW
	if (!initGLFW())
	{
		return false;
	}

	//Setting error callback
	glfwSetErrorCallback(error_callback);

	//Setting window properties and creating the window
	if (FrameworkConfig::api == OpenGL)
	{
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	}
	else
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, FrameworkConfig::apiVersion.x);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, FrameworkConfig::apiVersion.y);
	glfwWindowHint(GLFW_RESIZABLE, FrameworkConfig::windowResizable);
	glfwWindowHint(GLFW_SAMPLES, FrameworkConfig::windowNumSamples);

	window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	closed = false;

	//Checking if a valid window was created
	if (window == NULL)
	{
		PR_LOG_ERROR("The creation of the window has failed!\n");

		glfwTerminate();
		return false;
	}

	//Initialisation of the API
	if (!context->InitContext(this))
	{
		glfwTerminate();
		return false;
	}

	swapchain->SetupSwapchain(this);

	ImageData data = TextureLoader::LoadTextureData("res/textures/logo.png");

	GLFWimage image;
	image.width = data.width;
	image.height = data.height;
	image.pixels = data.data;

	glfwSetWindowIcon(window, 1, &image);

	glfwShowWindow(window);
	SetVSync(FrameworkConfig::windowVSync);

	return true;
}

WindowsWindow::~WindowsWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool WindowsWindow::initGLFW() const
{
	if (!glfwInit())
	{
		PR_LOG_ERROR("The initialisiation of OpenGL Framework (GLFW) has failed!\n");
		return false;
	}

	return true;
}

void WindowsWindow::Input()
{
	glfwPollEvents();
}

void WindowsWindow::Render() const
{
	swapchain->SwapBuffers();
}

bool WindowsWindow::ShouldClose() const
{
	return glfwWindowShouldClose(window) || closed;
}