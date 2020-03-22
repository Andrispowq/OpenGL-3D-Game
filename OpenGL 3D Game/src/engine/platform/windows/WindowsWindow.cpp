#include "engine/prehistoric/core/util/Includes.hpp"
#include "WindowsWindow.h"
#include "engine/platform/vulkan/framework/context/VkContext.h"
#include "engine/platform/vulkan/framework/swapchain/VkSwapChain.h"

WindowsWindow::~WindowsWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

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

	//Initialisation of GLEW
	if (!Context::GetContext().InitContext(this))
	{
		glfwTerminate();
		return false;
	}

	if (FrameworkConfig::api == OpenGL)
	{
		swapChain->SetupSwapChain(window, nullptr, nullptr);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		VkPhysicalDevice physicalDevice = dynamic_cast<VkContext&>(Context::GetContext()).GetPhysicalDeviceSelector().GetPhysicalDevice();
		VkDevice device = dynamic_cast<VkContext&>(Context::GetContext()).GetLogicalDeviceSelector().GetDevice();
		swapChain->SetupSwapChain(window, &physicalDevice, &device);
	}
	else
	{
		PR_LOG_RUNTIME_ERROR("Specified API is not valid!\n");
	}

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
	swapChain->SwapBuffers(window);
}

bool WindowsWindow::ShouldClose() const
{
	return glfwWindowShouldClose(window) || closed;
}