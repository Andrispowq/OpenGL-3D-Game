#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLSwapChain.h"
#include "engine/config/FrameworkConfig.h"

void GLSwapChain::SetupSwapChain(void* window, void* physicalDevice, void* device)
{
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
}

void GLSwapChain::SwapBuffers(void* window) const
{
	glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(window));
}

void GLSwapChain::SetVSync(bool vSync) const
{
	glfwSwapInterval(vSync ? 1 : 0);
}

void GLSwapChain::SetFullscreen(void* window, bool fullscreen) const
{
	glfwSetWindowMonitor(reinterpret_cast<GLFWwindow*>(window), fullscreen ? glfwGetPrimaryMonitor() : 0, 0, 0, FrameworkConfig::windowWidth, FrameworkConfig::windowHeight, 0);
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

void GLSwapChain::DeleteSwapChain(void* device)
{

}