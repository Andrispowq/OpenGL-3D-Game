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

const static uint32_t MAX_FRAMES_IN_FLIGHT = 3;

class VKSwapchain : public Swapchain
{
public:
	void SetupSwapchain(void* physicalDevice);

	void SwapBuffers(Window* window) override;

	void SetVSync(bool vSync) const override;
	void SetWindowSize(Window* window, uint32_t width, uint32_t height) override;

	void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha) override;
	void ClearScreen() override;

	void DeleteSwapchain(void* device) override;

	virtual uint32_t GetAquiredImageIndex() const override { return aquiredImageIndex; }

	VkSwapchainKHR GetSwapchain() const { return swapchain; }
	std::vector<VkImage> GetSwapchainImages() const { return swapchainImages; }
	std::vector<VkImageView> GetSwapchainImageViews() const { return swapchainImageViews; }
	VkFormat GetSwapchainImageFormat() const { return swapchainImageFormat; }
	VkExtent2D& GetSwapchainExtent() { return swapchainExtent; }

	VKCommandPool& GetCommandPool() { return *commandPool; }

	void RegisterSurface(VKSurface* surface) { this->surface = surface; }
	void RegisterDevice(VKDevice* device) { this->device = device; }
private:
	VKDevice* device;
	VKPhysicalDevice* physicalDev;
	VKSurface* surface;

	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapchainImages;
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;

	Vector4f clearColor;

	std::vector<VkImageView> swapchainImageViews;
	std::vector<VKSemaphore*> imageAvailableSemaphores;
	std::vector<VKSemaphore*> renderFinishedSemaphores;
	std::vector<VKFence*> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	uint32_t aquiredImageIndex;
	uint32_t NumImages;

	VKCommandPool* commandPool;
};

#endif
