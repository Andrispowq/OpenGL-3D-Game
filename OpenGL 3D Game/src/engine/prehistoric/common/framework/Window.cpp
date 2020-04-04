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
		swapchain = new GLSwapChain();
		context = new GLContext();
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		swapchain = new VKSwapchain();
		context = new VKContext();
	}
	else
	{
		PR_LOG_RUNTIME_ERROR("An invalid API has been specified under res/config/framework.cfg!\n");
	}
}

Window::Window()
{
	this->width = DEFAULT_WIDTH;
	this->height = DEFAULT_HEIGHT;
	this->title = DEFAULT_TITLE;
	this->fullscreen = DEFAULT_FULLSCREEN;
	this->closed = true;

	if (FrameworkConfig::api == OpenGL)
	{
		swapchain = new GLSwapChain();
		context = new GLContext();
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		swapchain = new VKSwapchain();
		context = new VKContext();
	}
	else
	{
		PR_LOG_RUNTIME_ERROR("An invalid API has been specified under res/config/framework.cfg!\n");
	}
}

Window::~Window()
{
	swapchain->DeleteSwapchain(context->GetDevice());
	context->DeleteContext(this);

	delete swapchain;
	delete context;
}