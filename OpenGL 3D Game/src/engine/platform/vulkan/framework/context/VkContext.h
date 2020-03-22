#ifndef VK_CONTEXT_H
#define VK_CONTEXT_H

#include "glew.h"
#define GLFW_INCLUDE_VULKAN
#include "vulkan/vulkan.h"

#include "VkDebugMessenger.h"
#include "engine/platform/vulkan/framework/device/VkDeviceSelector.h"
#include "engine/prehistoric/common/framework/context/Context.h"
#include "engine/platform/vulkan/framework/queue/VkQueueManager.h"
#include "engine/platform/vulkan/framework/swapchain/VkSwapChain.h"
#include "engine/config/FrameworkConfig.h"
#include "engine/platform/windows/WindowsWindow.h"

class VkContext : public Context
{
public:
	bool InitContext(Window* window) override; 
	bool DeleteContext(Window* window) override;

	VkPhysicalDeviceSelector GetPhysicalDeviceSelector() const { return physicalDeviceSelector; }
	VkDeviceSelector GetLogicalDeviceSelector() const { return logicalDeviceSelector; }
private:
	bool CheckValidationLayerSupport() const;
	std::vector<const char*> GetRequiredExtensions() const;
private:
	VkInstance instance;
	VkDebugMessenger messenger;

	VkPhysicalDeviceSelector physicalDeviceSelector;
	VkDeviceSelector logicalDeviceSelector;

	VkSurfaceKHR surface;

	std::vector<const char*> validationLayers;
};

#endif