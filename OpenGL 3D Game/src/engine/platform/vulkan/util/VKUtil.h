#pragma once

#include "engine/platform/vulkan/framework/surface/VKSurface.h"

#include "engine/prehistoric/core/util/Includes.hpp"
#include "engine/config/FrameworkConfig.h"

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
	QueueFamilyIndices FindQueueFamilies(VkSurfaceKHR& surface, VkPhysicalDevice& device);

	//Functions for swapchain creation
	SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR& surface, VkPhysicalDevice& device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	//Functions for buffer allocation
	uint32_t FindMemoryType(uint32_t typeFilter, VkPhysicalDevice& physicalDevice, VkMemoryPropertyFlags properties);
};