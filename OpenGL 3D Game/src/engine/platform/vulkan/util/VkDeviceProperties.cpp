#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkDeviceProperties.h"

void VkDeviceProperties::ListProperties(const void* device) const
{
	VkPhysicalDevice dev = *(reinterpret_cast<const VkPhysicalDevice*>(device));

	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceMemoryProperties memoryProps;

	vkGetPhysicalDeviceProperties(dev, &properties);
	vkGetPhysicalDeviceFeatures(dev, &deviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(dev, &memoryProps);

	PR_LOG_MESSAGE("Video card: %s\n", properties.deviceName);
	PR_LOG_MESSAGE("Video card type: %s\n", properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Dedicated GPU" : "Integrated GPU");

	PR_LOG_MESSAGE("Other important informations: \n");

	PR_LOG_MESSAGE("\tAmount of VRAM: %i MB, %i GB\n", memoryProps.memoryHeaps[0].size / 1024.0, memoryProps.memoryHeaps[0].size / (1024.0 * 1024.0));

	uint32_t totalSize = 0;
	for (VkMemoryHeap memSize : memoryProps.memoryHeaps)
	{
		totalSize += memSize.size;
	}
	PR_LOG_MESSAGE("\tAmount of VRAM and shared memory: %i MB, %i GB\n", totalSize / 1024.0, totalSize / (1024.0 * 1024.0));
}