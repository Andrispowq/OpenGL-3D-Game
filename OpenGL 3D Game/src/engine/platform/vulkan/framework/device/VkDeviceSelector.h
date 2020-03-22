#ifndef VK_DEVICE_SELECTOR
#define VK_DEVICE_SELECTOR

#include "VkPhysicalDeviceSelector.h"

#include "engine/platform/vulkan/framework/queue/VkQueueManager.h"

class VkDeviceSelector
{
public:
	VkDeviceSelector();
	virtual ~VkDeviceSelector();

	void CreateLogicalDevice(VkSurfaceKHR surface, const VkPhysicalDevice& physicalDevice, const std::vector<const char*>& validationLayers, const std::vector<const char*>& deviceExtensions);
	void DestroyLogicalDevice();

	VkDevice& GetDevice() { return device; }
private:
	VkDevice device;
	VkQueueManager graphicsQueue;
	VkQueueManager presentQueue;
};

#endif