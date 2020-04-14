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

class VKDevice;

static VkCommandPool copyCommandPool;

namespace VKUtil
{
	void Init(VkPhysicalDevice& physicalDevice, VkDevice& device);
	void CleanUp(VkDevice& device);

	QueueFamilyIndices FindQueueFamilies(VkSurfaceKHR& surface, VkPhysicalDevice& device);

	//Functions for swapchain creation
	SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR& surface, VkPhysicalDevice& device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkFormat FindSupportedFormat(VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	//Functions for buffer allocation
	uint32_t FindMemoryType(uint32_t typeFilter, VkPhysicalDevice& physicalDevice, VkMemoryPropertyFlags properties);
	void CreateBuffer(VkPhysicalDevice& physicalDevice, VkDevice& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VKDevice& device, VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size, VkCommandBuffer& buff);
	void CopyBuffer(VKDevice& device, VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size);

	void TransitionImageLayout(VKDevice& device, VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VKDevice& device, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);
};