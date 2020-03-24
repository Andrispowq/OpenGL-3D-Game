#ifndef VK_SURFACE_H
#define VK_SURFACE_H

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include <vulkan/vulkan.h>

#include "engine/prehistoric/common/framework/Window.h"
#include "engine/platform/vulkan/framework/context/VKInstance.h"

class VKSurface
{
public:
	void CreateSurface(Window* window, VKInstance* instance);
	void DeleteSurface(VKInstance* instance);

	VkSurfaceKHR& GetSurface() { return surface; }
private:
	VkSurfaceKHR surface;
};

#endif