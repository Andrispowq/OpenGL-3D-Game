#ifndef VK_SWAP_CHAIN_H
#define VK_SWAP_CHAIN_H

#include "engine/prehistoric/common/framework/swapchain/Swapchain.h"
#include "engine/prehistoric/core/util/Includes.hpp"

#include <glfw3.h>
#define PR_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/framework/surface/VKSurface.h"
#include "engine/platform/vulkan/rendering/command/VKCommandPool.h"
#include "engine/platform/vulkan/rendering/synchronization/VKSemaphore.h"
#include "engine/platform/vulkan/rendering/synchronization/VKFence.h"
#include "engine/platform/vulkan/rendering/renderpass/VKRenderpass.h"

const static uint32_t MAX_FRAMES_IN_FLIGHT = 3;

class VKSwapchain : public Swapchain
{
public:
	virtual void SetupSwapchain(Window* window);

	virtual void SwapBuffers() override;

	virtual void SetVSync(bool vSync) const override;
	virtual void SetWindowSize(uint32_t width, uint32_t height) override;

	virtual void SetClearColor(float red, float green, float blue, float alpha) override;
	virtual void ClearScreen() override;

	virtual void DeleteSwapchain(void* device) override;

	virtual uint32_t getAquiredImageIndex() const override { return aquiredImageIndex; }
	virtual void* getDrawCommandBuffer() const override { return commandPool->getCommandBuffer(aquiredImageIndex); }

	virtual void PrepareRendering() override;
	virtual void EndRendering() override;	

	VkSwapchainKHR getSwapchain() const { return swapchain; }
	std::vector<VkImage> getSwapchainImages() const { return swapchainImages; }
	std::vector<VkImageView> getSwapchainImageViews() const { return swapchainImageViews; }

	VkImageView& getDepthImageView() { return depthImageView; }
	VkFormat getSwapchainImageFormat() const { return swapchainImageFormat; }
	VkExtent2D getSwapchainExtent() const { return swapchainExtent; }

	VKCommandPool& getCommandPool() { return *commandPool; }
	VKRenderpass& getRenderpass() { return *renderpass; }

	void setSurface(VKSurface& surface) { this->surface = &surface; }
private:
	//External
	VKPhysicalDevice* physicalDevice;
	VKDevice* device;
	VKSurface* surface;

	//Clearing
	Vector4f clearColor;

	//Swapchain imageviews and images
	VkSwapchainKHR swapchain;
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;

	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;

	//For multisampling
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;
	
	//Depth buffer
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	//Synchronization
	std::vector<VKSemaphore*> imageAvailableSemaphores;
	std::vector<VKSemaphore*> renderFinishedSemaphores;
	std::vector<VKFence*> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	uint32_t aquiredImageIndex;
	uint32_t NumImages;

	//Rendering
	VKCommandPool* commandPool;
	VKRenderpass* renderpass;
	std::vector<VKFramebuffer*> swapchainFramebuffers;
};

#endif
