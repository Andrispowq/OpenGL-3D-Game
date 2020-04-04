#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKCapabilities.h"
#include "engine/platform/vulkan/framework/device/VKPhysicalDevice.h"

void VKCapabilities::QueryCapabilities(void* physicalDevice)
{
	VKPhysicalDevice* physicalDev = reinterpret_cast<VKPhysicalDevice*>(physicalDevice);

	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceMemoryProperties memoryProps;

	vkGetPhysicalDeviceProperties(physicalDev->GetPhysicalDevice(), &properties);
	vkGetPhysicalDeviceFeatures(physicalDev->GetPhysicalDevice(), &deviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(physicalDev->GetPhysicalDevice(), &memoryProps);

	physicalDeviceCaps.name = properties.deviceName;
	physicalDeviceCaps.discrete = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
	
	driverCaps.driverVersionMajor = VK_VERSION_MAJOR(properties.driverVersion);
	driverCaps.driverVersionMinor = VK_VERSION_MINOR(properties.driverVersion);
	driverCaps.driverVersionPatch = VK_VERSION_PATCH(properties.driverVersion);

	driverCaps.apiVersionMajor = VK_VERSION_MAJOR(properties.apiVersion);
	driverCaps.apiVersionMinor = VK_VERSION_MINOR(properties.apiVersion);
	driverCaps.apiVersionPatch = VK_VERSION_PATCH(properties.apiVersion);
	
	memoryCaps.vramSize = (uint32_t) memoryProps.memoryHeaps[0].size;

	VkMemoryHeap sharedSize = memoryProps.memoryHeaps[1];

	for (size_t i = 2; i < memoryProps.memoryHeapCount; i++)
	{
		sharedSize.size += memoryProps.memoryHeaps[i].size;
	}

	memoryCaps.sharedRamSize = (uint32_t) sharedSize.size;

	shaderCaps.geometryShaderSupported = deviceFeatures.geometryShader;
	shaderCaps.tessellationShaderSupported = deviceFeatures.tessellationShader;

	shaderCaps.computeShaderSupported = false;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDev->GetPhysicalDevice(), &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDev->GetPhysicalDevice(), &queueFamilyCount, queueFamilies.data());

	for (uint32_t i = 0; i < queueFamilyCount; i++)
	{
		if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			shaderCaps.computeShaderSupported = true;
			break;
		}
	}

	shaderCaps.rayTracingSupported = false;

	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDev->GetPhysicalDevice(), nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDev->GetPhysicalDevice(), nullptr, &extensionCount, availableExtensions.data());

	for (uint32_t i = 0; i < extensionCount; i++)
	{
		if (strcmp(availableExtensions[i].extensionName, VK_NV_RAY_TRACING_EXTENSION_NAME) == 0)
		{
			shaderCaps.rayTracingSupported = true;
			break;
		}
	}

	shaderCaps.meshShadingSupported = false;

	for (uint32_t i = 0; i < extensionCount; i++)
	{
		if (strcmp(availableExtensions[i].extensionName, VK_NV_MESH_SHADER_EXTENSION_NAME) == 0)
		{
			shaderCaps.meshShadingSupported = true;
			break;
		}
	}

	limits.maxTextureResolution = properties.limits.maxImageDimension2D;
	limits.maxTextureSlots = properties.limits.maxDescriptorSetSampledImages;
}