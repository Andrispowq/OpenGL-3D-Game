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
		PR_LOG_RUNTIME_ERROR("Failed to find GPUs with Vulkan support!\n");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, devices.data());

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	for (const auto& device : devices) 
	{
		if (IsDeviceSuitable(surface, device, deviceFeatures))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to find a suitable GPU!\n");
	}

	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

	if (counts & VK_SAMPLE_COUNT_1_BIT) msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	if (counts & VK_SAMPLE_COUNT_2_BIT) msaaSamples = VK_SAMPLE_COUNT_2_BIT;
	if (counts & VK_SAMPLE_COUNT_4_BIT) msaaSamples = VK_SAMPLE_COUNT_4_BIT;
	if (counts & VK_SAMPLE_COUNT_8_BIT) msaaSamples = VK_SAMPLE_COUNT_8_BIT;
	if (counts & VK_SAMPLE_COUNT_16_BIT) msaaSamples = VK_SAMPLE_COUNT_16_BIT;
	if (counts & VK_SAMPLE_COUNT_32_BIT) msaaSamples = VK_SAMPLE_COUNT_32_BIT;
	if (counts & VK_SAMPLE_COUNT_64_BIT) msaaSamples = VK_SAMPLE_COUNT_64_BIT;
}

bool VKPhysicalDevice::IsDeviceSuitable(VKSurface* surface, VkPhysicalDevice device, VkPhysicalDeviceFeatures features) const
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		QueueFamilyIndices indices = VKUtil::FindQueueFamilies(surface->GetSurface(), device);

		bool extensionsSupported = CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) 
		{
			SwapChainSupportDetails swapChainSupport = VKUtil::QuerySwapChainSupport(surface->GetSurface(), device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}
		
		return indices.hasGraphicsFamily && indices.hasPresentFamily && extensionsSupported && swapChainAdequate && deviceFeatures.samplerAnisotropy;
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