#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKSurface.h"
#include <memory>

void VKSurface::CreateSurface(Window* window, VKInstance* instance)
{
	VkResult res;
	if ((res = glfwCreateWindowSurface(instance->GetInstance(), (GLFWwindow*) window->GetWindowHandle(), nullptr, &surface)) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("The creation of VkSurfaceKHR has failed! Error: %i\n", res);
	}
}

void VKSurface::DeleteSurface(VKInstance* instance)
{
	vkDestroySurfaceKHR(instance->GetInstance(), surface, nullptr);
}