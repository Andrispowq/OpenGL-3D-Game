#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKPhysicalDevice.h"
#include "engine/config/FrameworkConfig.h"

VKPhysicalDevice::VKPhysicalDevice()
{
	physicalDevice = VK_NULL_HANDLE;

	deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

VKPhysicalDevice::~VKPhysicalDevice()
{
	physicalDevice = VK_NULL_HANDLE;
}

void VKPhysicalDevice::PickPhysicalDevice(VKSurface* surface, VKInstance* instance)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, nullptr);

	if (deviceCount == 0) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, devices.data());

	for (const auto& device : devices) 
	{
		if (IsDeviceSuitable(surface, device))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to find a suitable GPU!");
	}
}

bool VKPhysicalDevice::IsDeviceSuitable(VKSurface* surface, VkPhysicalDevice device) const
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		QueueFamilyIndices indices = VKUtil::FindQueueFamilies(surface, device);

		bool extensionsSupported = CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) 
		{
			SwapChainSupportDetails swapChainSupport = VKUtil::QuerySwapChainSupport(surface, device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.hasGraphicsFamily && indices.hasPresentFamily && extensionsSupported && swapChainAdequate;
	}
	else
	{
		return false;
	}
}

bool VKPhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device) const
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) 
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}