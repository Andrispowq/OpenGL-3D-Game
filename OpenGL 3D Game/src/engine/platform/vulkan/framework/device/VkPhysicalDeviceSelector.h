#ifndef VK_PHYSICAL_DEVICE_SELECTOR
#define VK_PHYSICAL_DEVICE_SELECTOR

#include "vulkan/vulkan.h"

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

class VkPhysicalDeviceSelector
{
public:
	VkPhysicalDeviceSelector();
	virtual ~VkPhysicalDeviceSelector();

	void PickPhysicalDevice(VkSurfaceKHR surface, VkInstance instance);
	static QueueFamilyIndices FindQueueFamilies(VkSurfaceKHR surface, VkPhysicalDevice device);

	//Functions for swapchain creation
	static SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR surface, VkPhysicalDevice device);
	static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
	std::vector<const char*> GetDeviceExtensions() const { return deviceExtensions; }
private:
	//Utility functions for picking GPU
	bool IsDeviceSuitable(VkSurfaceKHR surface, VkPhysicalDevice device) const;
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
private:
	VkPhysicalDevice physicalDevice;

	std::vector<const char*> deviceExtensions;
};

#endif