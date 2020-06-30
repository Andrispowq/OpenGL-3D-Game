#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKSwapchain.h"

#include "engine/platform/vulkan/framework/context/VKContext.h"

void VKSwapchain::SetupSwapchain(Window* window)
{
    this->window = window;

    this->physicalDevice = (VKPhysicalDevice*) window->GetContext()->GetPhysicalDevice();
    this->device = (VKDevice*)window->GetContext()->GetDevice();

    this->surface = &((VKContext*)window->GetContext())->GetSurface();

    SwapChainSupportDetails swapchainSupport = VKUtil::QuerySwapChainSupport(surface->GetSurface(), physicalDevice->GetPhysicalDevice());

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

    QueueFamilyIndices indices = VKUtil::FindQueueFamilies(surface->GetSurface(), physicalDevice->GetPhysicalDevice());
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
        VKUtil::CreateImageView(*device, swapchainImages[i], swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, swapchainImageViews[i]);
    }

    //Create multisampled image
    VKUtil::CreateImage(*physicalDevice, *device, swapchainExtent.width, swapchainExtent.height, 1, physicalDevice->getSampleCount(), swapchainImageFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        colorImage, colorImageMemory);
    VKUtil::CreateImageView(*device, colorImage, swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, colorImageView);

    //Create depth buffer
    VkFormat depthFormat = VKUtil::FindSupportedFormat(
        physicalDevice->GetPhysicalDevice(),
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

    VKUtil::CreateImage(*physicalDevice, *device, swapchainExtent.width, swapchainExtent.height, 1, physicalDevice->getSampleCount(), depthFormat, VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    VKUtil::CreateImageView(*device, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, depthImageView);

    VKUtil::TransitionImageLayout(*device, depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);

    //Create framebuffers and renderpass
    renderpass = new VKRenderpass(*physicalDevice, device->GetDevice(), swapchainImageFormat);
    
    swapchainFramebuffers.resize(swapchainImageViews.size());
    for (size_t i = 0; i < swapchainImageViews.size(); i++)
    {
        swapchainFramebuffers[i] = new VKFramebuffer(*renderpass, swapchainExtent, colorImageView, depthImageView, swapchainImageViews[i], device->GetDevice());
    }

    //Create command pool
    commandPool = new VKCommandPool(*surface, physicalDevice->GetPhysicalDevice(), device->GetDevice());

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

void VKSwapchain::PrepareRendering()
{
    renderpass->BeginRenderpass(*commandPool->GetCommandBuffer(aquiredImageIndex), swapchainExtent, *swapchainFramebuffers[aquiredImageIndex], clearColor);
    commandPool->GetCommandBuffer(aquiredImageIndex)->BindBuffer();

    //Get the next available image to render to
    vkWaitForFences(device->GetDevice(), 1, &(inFlightFences[currentFrame]->GetFence()), VK_TRUE, UINT64_MAX);

    VkResult res = vkAcquireNextImageKHR(device->GetDevice(), swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame]->GetSemaphore(), VK_NULL_HANDLE, &aquiredImageIndex);

    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
    {
        SetWindowSize(window->GetWidth(), window->GetHeight());
    }

    if (imagesInFlight[aquiredImageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device->GetDevice(), 1, &imagesInFlight[aquiredImageIndex], VK_TRUE, UINT64_MAX);
    }

    imagesInFlight[aquiredImageIndex] = inFlightFences[currentFrame]->GetFence();
}

void VKSwapchain::EndRendering()
{
    commandPool->GetCommandBuffer(aquiredImageIndex)->UnbindBuffer();
    renderpass->EndRenderpass(*commandPool->GetCommandBuffer(aquiredImageIndex));
}

void VKSwapchain::SwapBuffers()
{
    //Submit the graphics queue for rendering
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

    //Present the results on the present queue
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &aquiredImageIndex;
    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(device->GetPresentQueue().GetQueue(), &presentInfo);
    vkQueueWaitIdle(device->GetPresentQueue().GetQueue());

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VKSwapchain::SetVSync(bool vSync) const
{
    
}

void VKSwapchain::SetWindowSize(uint32_t width, uint32_t height)
{
    vkDeviceWaitIdle(device->GetDevice());

    //We destroy some objects that should be destroyed
    vkDestroyImageView(device->GetDevice(), depthImageView, nullptr);
    vkDestroyImage(device->GetDevice(), depthImage, nullptr);
    vkFreeMemory(device->GetDevice(), depthImageMemory, nullptr);

    vkDestroyImageView(device->GetDevice(), colorImageView, nullptr);
    vkDestroyImage(device->GetDevice(), colorImage, nullptr);
    vkFreeMemory(device->GetDevice(), colorImageMemory, nullptr);

    for (auto framebuffer : swapchainFramebuffers)
    {
        delete framebuffer;
    }

    commandPool->DeleteCommandBuffers();

    delete renderpass;

    for (size_t i = 0; i < NumImages; i++)
    {
        vkDestroyImageView(device->GetDevice(), swapchainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(device->GetDevice(), swapchain, nullptr);

    //Then we create new of them
    SwapChainSupportDetails swapchainSupport = VKUtil::QuerySwapChainSupport(surface->GetSurface(), physicalDevice->GetPhysicalDevice());

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

    QueueFamilyIndices indices = VKUtil::FindQueueFamilies(surface->GetSurface(), physicalDevice->GetPhysicalDevice());
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
        VKUtil::CreateImageView(*device, swapchainImages[i], swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, swapchainImageViews[i]);
    }

    //Create multisampled image
    VKUtil::CreateImage(*physicalDevice, *device, swapchainExtent.width, swapchainExtent.height, 1, physicalDevice->getSampleCount(), swapchainImageFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        colorImage, colorImageMemory);
    VKUtil::CreateImageView(*device, colorImage, swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, colorImageView);

    //Create depth buffer
    VkFormat depthFormat = VKUtil::FindSupportedFormat(
        physicalDevice->GetPhysicalDevice(),
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

    VKUtil::CreateImage(*physicalDevice, *device, swapchainExtent.width, swapchainExtent.height, 1, physicalDevice->getSampleCount(), depthFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    VKUtil::CreateImageView(*device, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, depthImageView);

    VKUtil::TransitionImageLayout(*device, depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);

    //Recreate renderpass
    renderpass = new VKRenderpass(*physicalDevice, device->GetDevice(), swapchainImageFormat);

    swapchainFramebuffers.resize(swapchainImageViews.size());
    for (size_t i = 0; i < swapchainImageViews.size(); i++)
    {
        swapchainFramebuffers[i] = new VKFramebuffer(*renderpass, swapchainExtent, colorImageView, depthImageView, swapchainImageViews[i], device->GetDevice());
    }

    //Reattach command buffers
    for (size_t i = 0; i < NumImages; i++)
    {
        commandPool->AddCommandBuffer();
    }
}

void VKSwapchain::SetClearColor(const float& red, const float& green, const float& blue, const float& alpha)
{
    clearColor = { red, green, blue, alpha };
}

void VKSwapchain::ClearScreen()
{
    /*VkClearColorValue value = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };

    VkImageSubresourceRange range = {};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.levelCount = 1;
    range.layerCount = 1;

    commandPool->GetCommandBuffer(aquiredImageIndex)->BindBuffer();
    vkCmdClearColorImage(commandPool->GetCommandBuffer(aquiredImageIndex)->GetCommandBuffer(), swapchainImages[aquiredImageIndex], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, &value, 1, &range);
    commandPool->GetCommandBuffer(aquiredImageIndex)->UnbindBuffer();*/
}

void VKSwapchain::DeleteSwapchain(void* device)
{
    VKDevice* dev = reinterpret_cast<VKDevice*>(device);

    vkDeviceWaitIdle(dev->GetDevice());

    for (auto framebuffer : swapchainFramebuffers)
    {
        delete framebuffer;
    }

    delete renderpass;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        delete renderFinishedSemaphores[i];
        delete imageAvailableSemaphores[i];
        delete inFlightFences[i];
    }

    delete commandPool;

    vkDestroyImageView(dev->GetDevice(), depthImageView, nullptr);
    vkDestroyImage(dev->GetDevice(), depthImage, nullptr);
    vkFreeMemory(dev->GetDevice(), depthImageMemory, nullptr);

    vkDestroyImageView(dev->GetDevice(), colorImageView, nullptr);
    vkDestroyImage(dev->GetDevice(), colorImage, nullptr);
    vkFreeMemory(dev->GetDevice(), colorImageMemory, nullptr);

    for (auto imageView : swapchainImageViews)
    {
        vkDestroyImageView(dev->GetDevice(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(dev->GetDevice(), swapchain, nullptr);
}