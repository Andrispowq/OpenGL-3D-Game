#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKSurface.h"

void VKSurface::CreateSurface(Window* window, VKInstance* instance)
{
	if (!glfwCreateWindowSurface(instance->GetInstance(), (GLFWwindow*) window->GetWindowHandle(), nullptr, &surface))
	{
		PR_LOG_RUNTIME_ERROR("The creation of VkSurfaceKHR has failed!\n");
	}
}

void VKSurface::DeleteSurface(VKInstance* instance)
{
	vkDestroySurfaceKHR(instance->GetInstance(), surface, nullptr);
}