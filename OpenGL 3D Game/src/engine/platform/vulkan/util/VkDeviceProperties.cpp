#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkDeviceProperties.h"
#include "engine/platform/vulkan/framework/device/VKPhysicalDevice.h"

void VKDeviceProperties::ListProperties(void* device) const
{
	VKPhysicalDevice* dev = reinterpret_cast<VKPhysicalDevice*>(device);

	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceMemoryProperties memoryProps;

	vkGetPhysicalDeviceProperties(dev->GetPhysicalDevice(), &properties);
	vkGetPhysicalDeviceFeatures(dev->GetPhysicalDevice(), &deviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(dev->GetPhysicalDevice(), &memoryProps);

	PR_LOG_MESSAGE("Video card: %s\n", properties.deviceName);
	PR_LOG_MESSAGE("Video card type: %s\n", properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Dedicated GPU" : "Integrated GPU");

	PR_LOG_MESSAGE("Other important informations: \n");

	PR_LOG_MESSAGE("\tAmount of VRAM: %i MB, %i GB\n\n", memoryProps.memoryHeaps[0].size / (1024 * 1024), memoryProps.memoryHeaps[0].size / (1024 * 1024 * 1024));
}