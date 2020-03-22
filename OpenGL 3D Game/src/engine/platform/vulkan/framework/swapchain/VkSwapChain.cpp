#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkSwapChain.h"
#include "engine/platform/vulkan/framework/device/VkPhysicalDeviceSelector.h"

void VkSwapChain::SetupSwapChain(void* window, void* physicalDevice, void* device)
{
    VkPhysicalDevice physicalDev = *(reinterpret_cast<VkPhysicalDevice*>(physicalDevice));
    VkDevice dev = *(reinterpret_cast<VkDevice*>(device));

    SwapChainSupportDetails swapChainSupport = VkPhysicalDeviceSelector::QuerySwapChainSupport(*surface, physicalDev);

    VkSurfaceFormatKHR surfaceFormat = VkPhysicalDeviceSelector::ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = VkPhysicalDeviceSelector::ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = VkPhysicalDeviceSelector::ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = *surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = VkPhysicalDeviceSelector::FindQueueFamilies(*surface, physicalDev);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else 
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(dev, &createInfo, nullptr, &swapChain) != VK_SUCCESS) 
    {
        PR_LOG_RUNTIME_ERROR("Failed to create swap chain!\n");
    }
}

void VkSwapChain::SwapBuffers(void* window) const
{

}

void VkSwapChain::SetVSync(bool vSync) const
{

}

void VkSwapChain::SetFullscreen(void* window, bool fullscreen) const
{

}

void VkSwapChain::SetClearColor(const float& red, const float& green, const float& blue, const float& alpha)
{

}

void VkSwapChain::ClearScreen()
{

}

void VkSwapChain::DeleteSwapChain(void* device)
{
    VkDevice dev = *(reinterpret_cast<VkDevice*>(device));

    vkDestroySwapchainKHR(dev, swapChain, nullptr);
}