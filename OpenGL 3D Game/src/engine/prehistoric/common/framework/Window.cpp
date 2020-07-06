#include "engine/prehistoric/core/util/Includes.hpp"
#include "Window.h"
#include "engine/prehistoric/common/framework/context/Context.h"
#include "engine/config/FrameworkConfig.h"
#include "engine/platform/opengl/framework/swapchain/GLSwapChain.h"
#include "engine/platform/vulkan/framework/swapchain/VkSwapChain.h"
#include "engine/platform/opengl/framework/context/GLContext.h"
#include "engine/platform/vulkan/framework/context/VKContext.h"

Window::Window(const int& width, const int& height, const char* title, const bool& fullscreen)
{
	this->width = width;
	this->height = height;
	this->title = title;
	this->fullscreen = fullscreen;
	this->closed = true;

	if (FrameworkConfig::api == OpenGL)
	{
		swapchain = new GLSwapchain();
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		swapchain = new VKSwapchain();
	}
	else
	{
		PR_LOG_RUNTIME_ERROR("An invalid API has been specified under res/config/framework.cfg!\n");
	}
}

Window::Window()
{
	this->width = FrameworkConfig::windowWidth;
	this->height = FrameworkConfig::windowHeight;
	this->title = FrameworkConfig::windowName.c_str();
	this->fullscreen = FrameworkConfig::windowFullScreen;
	this->closed = true;

	if (FrameworkConfig::api == OpenGL)
	{
		swapchain = new GLSwapchain();
		context = new GLContext(this);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		swapchain = new VKSwapchain();
		context = new VKContext(this);
	}
	else
	{
		PR_LOG_RUNTIME_ERROR("An invalid API has been specified under res/config/framework.cfg!\n");
	}
}

Window::~Window()
{
	swapchain->DeleteSwapchain(context->GetDevice());
	
	delete swapchain;
	delete context;
}