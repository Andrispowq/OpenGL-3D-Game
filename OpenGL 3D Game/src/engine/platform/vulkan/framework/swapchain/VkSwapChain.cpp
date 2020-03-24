#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKSwapchain.h"
#include "engine/platform/vulkan/framework/device/VKDevice.h"

void VKSwapchain::SetupSwapchain(void* physicalDevice, void* device)
{
    VKPhysicalDevice* physicalDev = reinterpret_cast<VKPhysicalDevice*>(physicalDevice);
    VKDevice* dev = reinterpret_cast<VKDevice*>(device);

    SwapChainSupportDetails swapChainSupport = VKUtil::QuerySwapChainSupport(surface, *physicalDev);

    VkSurfaceFormatKHR surfaceFormat = VKUtil::ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = VKUtil::ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = VKUtil::ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->GetSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = VKUtil::FindQueueFamilies(surface, *physicalDev);
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

    if (vkCreateSwapchainKHR(dev->GetDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to create swap chain!\n");
    }

    //Create images and image views
    vkGetSwapchainImagesKHR(dev->GetDevice(), swapchain, &imageCount, nullptr);

    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(dev->GetDevice(), swapchain, &imageCount, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;

    swapChainImageViews.resize(swapchainImages.size());

    for (size_t i = 0; i < swapchainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapchainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapchainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(dev->GetDevice(), &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
        {
            PR_LOG_RUNTIME_ERROR("Failed to create image views!\n");
        }
    }
}

void VKSwapchain::SwapBuffers(Window* window) const
{
    
}

void VKSwapchain::SetVSync(bool vSync) const
{
    
}

void VKSwapchain::SetFullscreen(Window* window, bool fullscreen) const
{

}

void VKSwapchain::SetClearColor(const float& red, const float& green, const float& blue, const float& alpha)
{

}

void VKSwapchain::ClearScreen()
{

}

void VKSwapchain::DeleteSwapchain(void* device)
{
    VkDevice dev = *(reinterpret_cast<VkDevice*>(device));

    for (auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(dev, imageView, nullptr);
    }

    vkDestroySwapchainKHR(dev, swapchain, nullptr);
}