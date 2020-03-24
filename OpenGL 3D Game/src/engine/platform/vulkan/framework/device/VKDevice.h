#ifndef VK_DEVICE_SELECTOR
#define VK_DEVICE_SELECTOR

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

	VkDevice GetDevice() const { return device; }
private:
	VkDevice device;

	VKQueue graphicsQueue;
	VKQueue presentQueue;
};

#endif