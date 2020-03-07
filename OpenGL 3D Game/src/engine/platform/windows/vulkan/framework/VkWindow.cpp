#include "VkWindow.h"
#include "context/VkContext.h"
#include <stdio.h>
#include "engine/config/FrameworkConfig.h"

VkWindow::~VkWindow()
{
	glfwDestroyWindow(window);
	Context::GetContext().DeleteContext();
	glfwTerminate();
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool VkWindow::Create()
{
	//Initialisation of GLFW
	if (!initGLFW())
	{
		return false;
	}

	//Setting error callback
	glfwSetErrorCallback(error_callback);

	//Setting window properties and creating the window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, FrameworkConfig::apiVersion.x);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, FrameworkConfig::apiVersion.y);
	glfwWindowHint(GLFW_RESIZABLE, FrameworkConfig::windowResizable);
	glfwWindowHint(GLFW_SAMPLES, FrameworkConfig::windowNumSamples);

	window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	closed = false;

	//Checking if a valid window was created
	if (window == NULL)
	{
		std::cout << "The creation of the window has failed!" << std::endl;

		glfwTerminate();
		return false;
	}

	//Initialisation of GLEW
	if (!Context::GetContext().InitContext())
	{
		glfwTerminate();
		return false;
	}

	glfwShowWindow(window);
	glfwSwapInterval(1);

	return true;
}

bool VkWindow::initGLFW() const
{
	if (!glfwInit())
	{
		std::cout << "The initialisiation of OpenGL Framework (GLFW) has failed!" << std::endl;
		return false;
	}

	return true;
}

void VkWindow::ClearColor(const float& red, const float& green, const float& blue, const float& alpha) const
{
	//glClearColor(red, green, blue, alpha);
}

void VkWindow::Input()
{
	glfwPollEvents();
}

void VkWindow::ClearScreen() const
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void VkWindow::Render() const
{
	glfwSwapBuffers(window);
}

bool VkWindow::ShouldClose() const
{
	return glfwWindowShouldClose(window) || closed;
}