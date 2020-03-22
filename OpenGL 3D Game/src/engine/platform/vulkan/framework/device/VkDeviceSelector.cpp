#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkDeviceSelector.h"
#include "engine/config/FrameworkConfig.h"

VkDeviceSelector::VkDeviceSelector()
{
	device = VK_NULL_HANDLE;
}

VkDeviceSelector::~VkDeviceSelector()
{
	device = VK_NULL_HANDLE;
}

void VkDeviceSelector::CreateLogicalDevice(VkSurfaceKHR surface, const VkPhysicalDevice& physicalDevice, const std::vector<const char*>& validationLayers, const std::vector<const char*>& deviceExtensions)
{
	QueueFamilyIndices indices = VkPhysicalDeviceSelector::FindQueueFamilies(surface, physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (FrameworkConfig::apiVulkanUseValidationLayers) 
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else 
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to create logical device!");
	}

	graphicsQueue.SetupQueue(device, indices.graphicsFamily);
	presentQueue.SetupQueue(device, indices.presentFamily);
}

void VkDeviceSelector::DestroyLogicalDevice()
{
	vkDestroyDevice(device, nullptr);
}