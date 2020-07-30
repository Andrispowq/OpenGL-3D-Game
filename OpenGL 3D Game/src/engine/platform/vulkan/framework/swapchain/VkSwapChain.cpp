#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKSwapchain.h"

#include "engine/platform/vulkan/framework/context/VKContext.h"

void VKSwapchain::SetupSwapchain(Window* window)
{
    this->window = window;

    this->physicalDevice = (VKPhysicalDevice*) window->getContext()->getPhysicalDevice();
    this->device = (VKDevice*)window->getContext()->getDevice();

    this->surface = &((VKContext*)window->getContext())->getSurface();

    SwapChainSupportDetails swapchainSupport = VKUtil::QuerySwapChainSupport(physicalDevice->getPhysicalDevice(), surface->getSurface());

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
    createInfo.surface = surface->getSurface();
    createInfo.minImageCount = NumImages;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = VKUtil::FindQueueFamilies(physicalDevice->getPhysicalDevice(), surface->getSurface());
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

    if (vkCreateSwapchainKHR(device->getDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to create swap chain!\n");
    }

    //Create images and image views
    vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &NumImages, nullptr);

    swapchainImages.resize(NumImages);
    vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &NumImages, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;

    swapchainImageViews.resize(NumImages);

    for (size_t i = 0; i < NumImages; i++)
    {
        VKUtil::CreateImageView(device, swapchainImages[i], swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, swapchainImageViews[i]);
    }

    //Create multisampled image
    VKUtil::CreateImage(physicalDevice->getPhysicalDevice(), device, swapchainExtent.width, swapchainExtent.height, 1, physicalDevice->getSampleCount(), swapchainImageFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        colorImage, colorImageMemory);
    VKUtil::CreateImageView(device, colorImage, swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, colorImageView);

    //Create depth buffer
    VkFormat depthFormat = VKUtil::FindSupportedFormat(
        physicalDevice->getPhysicalDevice(),
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

    VKUtil::CreateImage(physicalDevice->getPhysicalDevice(), device, swapchainExtent.width, swapchainExtent.height, 1, physicalDevice->getSampleCount(), depthFormat, VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    VKUtil::CreateImageView(device, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, depthImageView);

    VKUtil::TransitionImageLayout(device, depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);

    //Create framebuffers and renderpass
    renderpass = new VKRenderpass(physicalDevice, device->getDevice(), swapchainImageFormat);
    
    swapchainFramebuffers.resize(swapchainImageViews.size());
    for (size_t i = 0; i < swapchainImageViews.size(); i++)
    {
        swapchainFramebuffers[i] = new VKFramebuffer(renderpass, device->getDevice(), swapchainExtent, colorImageView, depthImageView, swapchainImageViews[i]);
    }

    //Create command pool
    commandPool = new VKCommandPool(physicalDevice->getPhysicalDevice(), device->getDevice(), surface);

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
        imageAvailableSemaphores[i] = new VKSemaphore(device->getDevice());
        renderFinishedSemaphores[i] = new VKSemaphore(device->getDevice());
        inFlightFences[i] = new VKFence(device->getDevice());
    }

    vkAcquireNextImageKHR(device->getDevice(), swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame]->getSemaphore(), VK_NULL_HANDLE, &aquiredImageIndex);
    
    if (imagesInFlight[aquiredImageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device->getDevice(), 1, &imagesInFlight[aquiredImageIndex], VK_TRUE, UINT64_MAX);
    }

    imagesInFlight[aquiredImageIndex] = inFlightFences[currentFrame]->getFence();
}

void VKSwapchain::PrepareRendering()
{
    //get the next available image to render to
    vkWaitForFences(device->getDevice(), 1, &(inFlightFences[currentFrame]->getFence()), VK_TRUE, UINT64_MAX);

    VkResult res = vkAcquireNextImageKHR(device->getDevice(), swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame]->getSemaphore(), VK_NULL_HANDLE, &aquiredImageIndex);

    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
    {
        SetWindowSize(window->getWidth(), window->getHeight());
    }

    if (imagesInFlight[aquiredImageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device->getDevice(), 1, &imagesInFlight[aquiredImageIndex], VK_TRUE, UINT64_MAX);
    }

    imagesInFlight[aquiredImageIndex] = inFlightFences[currentFrame]->getFence();

    commandPool->getCommandBuffer(aquiredImageIndex)->BindBuffer();
    renderpass->BeginRenderpass(commandPool->getCommandBuffer(aquiredImageIndex), swapchainFramebuffers[aquiredImageIndex], swapchainExtent, clearColor);
}

void VKSwapchain::EndRendering()
{
    renderpass->EndRenderpass(commandPool->getCommandBuffer(aquiredImageIndex));
    commandPool->getCommandBuffer(aquiredImageIndex)->UnbindBuffer();
}

void VKSwapchain::SwapBuffers()
{
    //Submit the graphics queue for rendering
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame]->getSemaphore() };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandPool->getCommandBuffer(aquiredImageIndex)->getCommandBuffer();

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame]->getSemaphore() };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    inFlightFences[currentFrame]->ResetFence();

    if (vkQueueSubmit(device->getGraphicsQueue().getQueue(), 1, &submitInfo, inFlightFences[currentFrame]->getFence()) != VK_SUCCESS)
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

    vkQueuePresentKHR(device->getPresentQueue().getQueue(), &presentInfo);
    vkQueueWaitIdle(device->getPresentQueue().getQueue());

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VKSwapchain::SetVSync(bool vSync) const
{
    
}

void VKSwapchain::SetWindowSize(uint32_t width, uint32_t height)
{
    vkDeviceWaitIdle(device->getDevice());

    //We destroy some objects that should be destroyed
    vkDestroyImageView(device->getDevice(), depthImageView, nullptr);
    vkDestroyImage(device->getDevice(), depthImage, nullptr);
    vkFreeMemory(device->getDevice(), depthImageMemory, nullptr);

    vkDestroyImageView(device->getDevice(), colorImageView, nullptr);
    vkDestroyImage(device->getDevice(), colorImage, nullptr);
    vkFreeMemory(device->getDevice(), colorImageMemory, nullptr);

    for (auto framebuffer : swapchainFramebuffers)
    {
        delete framebuffer;
    }

    commandPool->DeleteCommandBuffers();

    delete renderpass;

    for (size_t i = 0; i < NumImages; i++)
    {
        vkDestroyImageView(device->getDevice(), swapchainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(device->getDevice(), swapchain, nullptr);

    //Then we create new of them
    SwapChainSupportDetails swapchainSupport = VKUtil::QuerySwapChainSupport(physicalDevice->getPhysicalDevice(), surface->getSurface());

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
    createInfo.surface = surface->getSurface();
    createInfo.minImageCount = NumImages;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = VKUtil::FindQueueFamilies(physicalDevice->getPhysicalDevice(), surface->getSurface());
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

    if (vkCreateSwapchainKHR(device->getDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
    {
        PR_LOG_RUNTIME_ERROR("Failed to create swap chain!\n");
    }

    //Create images and image views
    vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &NumImages, nullptr);

    swapchainImages.resize(NumImages);
    vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &NumImages, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;

    swapchainImageViews.resize(NumImages);

    for (size_t i = 0; i < NumImages; i++)
    {
        VKUtil::CreateImageView(device, swapchainImages[i], swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, swapchainImageViews[i]);
    }

    //Create multisampled image
    VKUtil::CreateImage(physicalDevice->getPhysicalDevice(), device, swapchainExtent.width, swapchainExtent.height, 1, physicalDevice->getSampleCount(), swapchainImageFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        colorImage, colorImageMemory);
    VKUtil::CreateImageView(device, colorImage, swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, colorImageView);

    //Create depth buffer
    VkFormat depthFormat = VKUtil::FindSupportedFormat(
        physicalDevice->getPhysicalDevice(),
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

    VKUtil::CreateImage(physicalDevice->getPhysicalDevice(), device, swapchainExtent.width, swapchainExtent.height, 1, physicalDevice->getSampleCount(), depthFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    VKUtil::CreateImageView(device, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, depthImageView);

    VKUtil::TransitionImageLayout(device, depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);

    //Recreate renderpass
    renderpass = new VKRenderpass(physicalDevice, device->getDevice(), swapchainImageFormat);

    swapchainFramebuffers.resize(swapchainImageViews.size());
    for (size_t i = 0; i < swapchainImageViews.size(); i++)
    {
        swapchainFramebuffers[i] = new VKFramebuffer(renderpass, device->getDevice(), swapchainExtent, colorImageView, depthImageView, swapchainImageViews[i]);
    }

    //Reattach command buffers
    for (size_t i = 0; i < NumImages; i++)
    {
        commandPool->AddCommandBuffer();
    }
}

void VKSwapchain::SetClearColor(float red, float green, float blue, float alpha)
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

    commandPool->getCommandBuffer(aquiredImageIndex)->BindBuffer();
    vkCmdClearColorImage(commandPool->getCommandBuffer(aquiredImageIndex)->getCommandBuffer(), swapchainImages[aquiredImageIndex], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, &value, 1, &range);
    commandPool->getCommandBuffer(aquiredImageIndex)->UnbindBuffer();*/
}

void VKSwapchain::DeleteSwapchain(void* device)
{
    VKDevice* dev = reinterpret_cast<VKDevice*>(device);

    vkDeviceWaitIdle(dev->getDevice());

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

    vkDestroyImageView(dev->getDevice(), depthImageView, nullptr);
    vkDestroyImage(dev->getDevice(), depthImage, nullptr);
    vkFreeMemory(dev->getDevice(), depthImageMemory, nullptr);

    vkDestroyImageView(dev->getDevice(), colorImageView, nullptr);
    vkDestroyImage(dev->getDevice(), colorImage, nullptr);
    vkFreeMemory(dev->getDevice(), colorImageMemory, nullptr);

    for (auto imageView : swapchainImageViews)
    {
        vkDestroyImageView(dev->getDevice(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(dev->getDevice(), swapchain, nullptr);
}