#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLSwapChain.h"
#include "engine/config/FrameworkConfig.h"
#include "engine/prehistoric/common/framework/Window.h"

void GLSwapChain::SetupSwapchain(void* physicalDevice)
{
	//Enabling GL_TEXTURE_2D is deprecated, in debug mode, it will cause the program to break
	//glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
}

void GLSwapChain::SwapBuffers(Window* window)
{
	glfwSwapBuffers((GLFWwindow*)window->GetWindowHandle());
}

void GLSwapChain::SetVSync(bool vSync) const
{
	glfwSwapInterval(vSync ? 1 : 0);
}

void GLSwapChain::SetWindowSize(Window* window, uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
	FrameworkConfig::windowWidth = width;
	FrameworkConfig::windowHeight = height;
}

void GLSwapChain::SetClearColor(const float& red, const float& green, const float& blue, const float& alpha)
{
	glClearColor(red, green, blue, alpha);
}

void GLSwapChain::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0);
}

void GLSwapChain::DeleteSwapchain(void* device)
{

}