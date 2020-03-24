#pragma once

#include "engine/platform/vulkan/framework/surface/VKSurface.h"

#include "engine/prehistoric/core/util/Includes.hpp"
#include "engine/config/FrameworkConfig.h"

#include <vulkan/vulkan.h>

struct QueueFamilyIndices
{
	uint32_t graphicsFamily;
	bool hasGraphicsFamily;

	uint32_t presentFamily;
	bool hasPresentFamily;
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VKPhysicalDevice;

namespace VKUtil
{
	QueueFamilyIndices FindQueueFamilies(VKSurface* surface, const VKPhysicalDevice& device);

	//Functions for swapchain creation
	SwapChainSupportDetails QuerySwapChainSupport(VKSurface* surface, const VKPhysicalDevice& device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};