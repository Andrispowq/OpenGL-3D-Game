#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKSwapchain.h"

void VKSwapchain::SetupSwapchain(void* physicalDevice)
{
    VKPhysicalDevice* physicalDev = reinterpret_cast<VKPhysicalDevice*>(physicalDevice);
    this->physicalDev = physicalDev;

    SwapChainSupportDetails swapchainSupport = VKUtil::QuerySwapChainSupport(surface->GetSurface(), physicalDev->GetPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = VKUtil::ChooseSwapSurfaceFormat(swapchainSupport.formats);
    VkPresentModeKHR presentMode = VKUtil::ChooseSwapPresentMode(swapchainSupport.presentModes);
    VkExtent2D extent = VKUtil::ChooseSwapExtent(swapchainSupport.capabilities);

    NumImages = swapchainSupport.capabilities.minImageCount + 1;

    if (swapchainSupport.capabilities.maxImageCount > 0 && NumImages > swapchainSupport.capabilities.maxImageCount)
    {
        NumImages = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->GetSurface();
    createInfo.minImageCount = NumImages;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = VKUtil::FindQueueFamilies(surface->GetSurface(), physicalDev->GetPhysicalDevice());
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

    createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device->GetDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to create swap chain!\n");
    }

    //Create images and image views
    vkGetSwapchainImagesKHR(device->GetDevice(), swapchain, &NumImages, nullptr);

    swapchainImages.resize(NumImages);
    vkGetSwapchainImagesKHR(device->GetDevice(), swapchain, &NumImages, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;

    swapchainImageViews.resize(NumImages);

    for (size_t i = 0; i < NumImages; i++)
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

        if (vkCreateImageView(device->GetDevice(), &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS)
        {
            PR_LOG_RUNTIME_ERROR("Failed to create image views!\n");
        }
    }

    //Create depth buffer
    VkFormat depthFormat = VKUtil::FindSupportedFormat(
        physicalDev->GetPhysicalDevice(),
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = swapchainExtent.width;
    imageInfo.extent.height = swapchainExtent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = depthFormat;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional

    if (vkCreateImage(device->GetDevice(), &imageInfo, nullptr, &depthImage) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to create image!\n");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device->GetDevice(), depthImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = VKUtil::FindMemoryType(memRequirements.memoryTypeBits, physicalDev->GetPhysicalDevice(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &depthImageMemory) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to allocate image memory!\n");
    }

    vkBindImageMemory(device->GetDevice(), depthImage, depthImageMemory, 0);

    VkImageViewCreateInfo depthImageViewCreateInfo = {};
    depthImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthImageViewCreateInfo.image = depthImage;
    depthImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthImageViewCreateInfo.format = depthFormat;

    depthImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    depthImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    depthImageViewCreateInfo.subresourceRange.levelCount = 1;
    depthImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    depthImageViewCreateInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device->GetDevice(), &depthImageViewCreateInfo, nullptr, &depthImageView) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to create image views!\n");
    }

    VKUtil::TransitionImageLayout(*device, depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    //Create command pool
    commandPool = new VKCommandPool(*surface, physicalDev->GetPhysicalDevice(), device->GetDevice());

    for (size_t i = 0; i < NumImages; i++)
    {
        commandPool->AddCommandBuffer();
    }

    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(NumImages, VK_NULL_HANDLE);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        imageAvailableSemaphores[i] = new VKSemaphore(device->GetDevice());
        renderFinishedSemaphores[i] = new VKSemaphore(device->GetDevice());
        inFlightFences[i] = new VKFence(device->GetDevice());
    }

    vkAcquireNextImageKHR(device->GetDevice(), swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame]->GetSemaphore(), VK_NULL_HANDLE, &aquiredImageIndex);
    
    if (imagesInFlight[aquiredImageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device->GetDevice(), 1, &imagesInFlight[aquiredImageIndex], VK_TRUE, UINT64_MAX);
    }

    imagesInFlight[aquiredImageIndex] = inFlightFences[currentFrame]->GetFence();
}

void VKSwapchain::SwapBuffers(Window* window)
{
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame]->GetSemaphore() };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandPool->GetCommandBuffer(aquiredImageIndex)->GetCommandBuffer();

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame]->GetSemaphore() };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    inFlightFences[currentFrame]->ResetFence();

    if (vkQueueSubmit(device->GetGraphicsQueue().GetQueue(), 1, &submitInfo, inFlightFences[currentFrame]->GetFence()) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to submit draw command buffer!\n");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapchain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &aquiredImageIndex;
    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(device->GetPresentQueue().GetQueue(), &presentInfo);

    vkQueueWaitIdle(device->GetPresentQueue().GetQueue());

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    //After drawing, we can get the next image to draw on
    vkWaitForFences(device->GetDevice(), 1, &(inFlightFences[currentFrame]->GetFence()), VK_TRUE, UINT64_MAX);

    VkResult res = vkAcquireNextImageKHR(device->GetDevice(), swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame]->GetSemaphore(), VK_NULL_HANDLE, &aquiredImageIndex);

    if (res == VK_ERROR_OUT_OF_DATE_KHR)
    {
        SetWindowSize(window, window->GetWidth(), window->GetHeight());
    }

    if (imagesInFlight[aquiredImageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device->GetDevice(), 1, &imagesInFlight[aquiredImageIndex], VK_TRUE, UINT64_MAX);
    }

    imagesInFlight[aquiredImageIndex] = inFlightFences[currentFrame]->GetFence();
}

void VKSwapchain::SetVSync(bool vSync) const
{
    
}

void VKSwapchain::SetWindowSize(Window* window, uint32_t width, uint32_t height)
{
    FrameworkConfig::windowWidth = width;
    FrameworkConfig::windowHeight = height;

    vkDeviceWaitIdle(device->GetDevice());

    //We destroy some objects that should be destroyed
    commandPool->DeleteCommandBuffers();

    vkDestroyImageView(device->GetDevice(), depthImageView, nullptr);
    vkDestroyImage(device->GetDevice(), depthImage, nullptr);
    vkFreeMemory(device->GetDevice(), depthImageMemory, nullptr);

    for (size_t i = 0; i < NumImages; i++)
    {
        vkDestroyImageView(device->GetDevice(), swapchainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(device->GetDevice(), swapchain, nullptr);

    //Then we create new of them
    SwapChainSupportDetails swapchainSupport = VKUtil::QuerySwapChainSupport(surface->GetSurface(), physicalDev->GetPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = VKUtil::ChooseSwapSurfaceFormat(swapchainSupport.formats);
    VkPresentModeKHR presentMode = VKUtil::ChooseSwapPresentMode(swapchainSupport.presentModes);
    VkExtent2D extent = VKUtil::ChooseSwapExtent(swapchainSupport.capabilities);

    NumImages = swapchainSupport.capabilities.minImageCount + 1;

    if (swapchainSupport.capabilities.maxImageCount > 0 && NumImages > swapchainSupport.capabilities.maxImageCount)
    {
        NumImages = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->GetSurface();
    createInfo.minImageCount = NumImages;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = VKUtil::FindQueueFamilies(surface->GetSurface(), physicalDev->GetPhysicalDevice());
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

    createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device->GetDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to create swap chain!\n");
    }

    //Create images and image views
    vkGetSwapchainImagesKHR(device->GetDevice(), swapchain, &NumImages, nullptr);

    swapchainImages.resize(NumImages);
    vkGetSwapchainImagesKHR(device->GetDevice(), swapchain, &NumImages, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;

    swapchainImageViews.resize(NumImages);

    for (size_t i = 0; i < NumImages; i++)
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

        if (vkCreateImageView(device->GetDevice(), &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS)
        {
            PR_LOG_RUNTIME_ERROR("Failed to create image views!\n");
        }
    }

    //Create depth buffer
    VkFormat depthFormat = VKUtil::FindSupportedFormat(
        physicalDev->GetPhysicalDevice(),
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = swapchainExtent.width;
    imageInfo.extent.height = swapchainExtent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = depthFormat;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional

    if (vkCreateImage(device->GetDevice(), &imageInfo, nullptr, &depthImage) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to create image!\n");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device->GetDevice(), depthImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = VKUtil::FindMemoryType(memRequirements.memoryTypeBits, physicalDev->GetPhysicalDevice(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &depthImageMemory) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to allocate image memory!\n");
    }

    vkBindImageMemory(device->GetDevice(), depthImage, depthImageMemory, 0);

    VkImageViewCreateInfo depthImageViewCreateInfo = {};
    depthImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthImageViewCreateInfo.image = depthImage;
    depthImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthImageViewCreateInfo.format = depthFormat;

    depthImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    depthImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    depthImageViewCreateInfo.subresourceRange.levelCount = 1;
    depthImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    depthImageViewCreateInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device->GetDevice(), &depthImageViewCreateInfo, nullptr, &depthImageView) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to create image views!\n");
    }

    VKUtil::TransitionImageLayout(*device, depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    for (size_t i = 0; i < NumImages; i++)
    {
        commandPool->AddCommandBuffer();
    }

    /*vkAcquireNextImageKHR(device->GetDevice(), swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame]->GetSemaphore(), VK_NULL_HANDLE, &aquiredImageIndex);
    
    if (imagesInFlight[aquiredImageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device->GetDevice(), 1, &imagesInFlight[aquiredImageIndex], VK_TRUE, UINT64_MAX);
    }

    imagesInFlight[aquiredImageIndex] = inFlightFences[currentFrame]->GetFence();*/
}

void VKSwapchain::SetClearColor(const float& red, const float& green, const float& blue, const float& alpha)
{
    clearColor = { red, green, blue, alpha };
}

void VKSwapchain::ClearScreen()
{
    VkClearColorValue value = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };

    VkImageSubresourceRange range = {};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.levelCount = 1;
    range.layerCount = 1;

    for (size_t i = 0; i < NumImages; i++)
    {
        //commandPool->BindCommandBuffer(i);

        //vkCmdClearColorImage(commandPool->GetCommandBuffer(i)->GetCommandBuffer(), swapchainImages[i], VK_IMAGE_LAYOUT_GENERAL, &value, 1, &range);
    
        //commandPool->UnbindCommandBuffer(i);
    }
}

void VKSwapchain::DeleteSwapchain(void* device)
{
    VKDevice* dev = reinterpret_cast<VKDevice*>(device);

    vkDeviceWaitIdle(dev->GetDevice());

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        delete renderFinishedSemaphores[i];
        delete imageAvailableSemaphores[i];
        delete inFlightFences[i];
    }

    delete commandPool;

    for (auto imageView : swapchainImageViews)
    {
        vkDestroyImageView(dev->GetDevice(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(dev->GetDevice(), swapchain, nullptr);
}