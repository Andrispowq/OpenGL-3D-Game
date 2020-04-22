#ifndef VK_SURFACE_H
#define VK_SURFACE_H

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "engine/prehistoric/common/framework/Window.h"
#include "engine/platform/vulkan/framework/context/VKInstance.h"

class VKSurface
{
public:
	void CreateSurface(Window* window, VKInstance* instance);
	void DeleteSurface();

	VkSurfaceKHR& GetSurface() { return surface; }
private:
	VKInstance* instance;

	VkSurfaceKHR surface;
};

#endif