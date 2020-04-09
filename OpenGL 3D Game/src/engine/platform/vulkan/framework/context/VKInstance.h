#ifndef VK_INSTANCE_H
#define VK_INSTANCE_H

//GLFW is only included for getting the VK_KHR_win32_surface extensions for windows
#include <vulkan/vulkan.h>
#define PR_INCLUDE_VULKAN
#include <glfw3.h>

#include "VKDebugMessenger.h"
#include "engine/config/FrameworkConfig.h"

class VKInstance
{
public:
	void CreateInstance();
	void DeleteInstance();

	VkInstance GetInstance() const { return instance; }
	VKDebugMessenger GetDebugMessenger() const { return messenger; }

	std::vector<const char*> GetValidationLayers() const { return validationLayers; }
private:
	bool CheckValidationLayerSupport() const;
	std::vector<const char*> GetRequiredExtensions() const;

	void ListExtensions() const;
private:
	VkInstance instance;
	VKDebugMessenger messenger;

	std::vector<const char*> validationLayers;
};

#endif