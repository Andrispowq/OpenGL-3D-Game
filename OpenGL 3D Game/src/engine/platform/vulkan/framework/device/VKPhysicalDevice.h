#ifndef VK_PHYSICAL_DEVICE_SELECTOR
#define VK_PHYSICAL_DEVICE_SELECTOR

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/surface/VKSurface.h"
#include "engine/platform/vulkan/framework/context/VKInstance.h"

#include "engine/platform/vulkan/util/VKUtil.h"

class VKPhysicalDevice
{
public:
	VKPhysicalDevice();
	virtual ~VKPhysicalDevice();

	void PickPhysicalDevice(VKSurface* surface, VKInstance* instance);

	VkPhysicalDevice& GetPhysicalDevice() { return physicalDevice; }
	std::vector<const char*> GetDeviceExtensions() const { return deviceExtensions; }

	VkSampleCountFlagBits getSampleCount() const { return msaaSamples; }
private:
	//Utility functions for picking GPU
	bool IsDeviceSuitable(VKSurface* surface, VkPhysicalDevice device, VkPhysicalDeviceFeatures features) const;
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
private:
	VkPhysicalDevice physicalDevice;

	std::vector<const char*> deviceExtensions;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};

#endif