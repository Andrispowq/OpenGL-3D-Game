#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKUtil.h"
#include "engine/platform/vulkan/framework/device/VKPhysicalDevice.h"

namespace VKUtil
{
	QueueFamilyIndices VKUtil::FindQueueFamilies(VKSurface* surface, const VKPhysicalDevice& device)
	{
		QueueFamilyIndices indices = { 0, false };

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device.GetPhysicalDevice(), &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device.GetPhysicalDevice(), &queueFamilyCount, queueFamilies.data());

		int i = 0;

		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
				indices.hasGraphicsFamily = true;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device.GetPhysicalDevice(), i, surface->GetSurface(), &presentSupport);

			if (presentSupport)
			{
				indices.presentFamily = i;
				indices.hasPresentFamily = true;
			}

			if (indices.hasGraphicsFamily && indices.hasPresentFamily)
			{
				break;
			}

			i++;
		}

		return indices;
	}

	SwapChainSupportDetails VKUtil::QuerySwapChainSupport(VKSurface* surface, const VKPhysicalDevice& device)
	{
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.GetPhysicalDevice(), surface->GetSurface(), &details.capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetPhysicalDevice(), surface->GetSurface(), &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetPhysicalDevice(), surface->GetSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device.GetPhysicalDevice(), surface->GetSurface(), &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device.GetPhysicalDevice(), surface->GetSurface(), &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR VKUtil::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VKUtil::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VKUtil::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { FrameworkConfig::windowWidth, FrameworkConfig::windowHeight };

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}
};