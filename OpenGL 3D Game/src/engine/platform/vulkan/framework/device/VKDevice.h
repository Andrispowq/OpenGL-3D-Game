#ifndef VK_DEVICE_SELECTOR
#define VK_DEVICE_SELECTOR

#include <vulkan/vulkan.h>

#include "VKPhysicalDevice.h"
#include "engine/platform/vulkan/framework/surface/VKSurface.h"
#include "engine/platform/vulkan/framework/queue/VKQueue.h"

class VKDevice
{
public:
	VKDevice();
	virtual ~VKDevice();

	void CreateLogicalDevice(VKSurface* surface, VKPhysicalDevice* physicalDevice, const std::vector<const char*>& validationLayers, const std::vector<const char*>& deviceExtensions);
	void DestroyLogicalDevice();

	VkDevice& GetDevice() { return device; }

	VKQueue& GetGraphicsQueue() { return graphicsQueue; }
	VKQueue& GetPresentQueue() { return presentQueue; }
private:
	VkDevice device;

	VKQueue graphicsQueue;
	VKQueue presentQueue;
};

#endif