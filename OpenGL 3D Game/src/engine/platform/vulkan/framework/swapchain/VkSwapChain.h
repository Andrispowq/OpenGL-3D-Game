#ifndef VK_SWAP_CHAIN_H
#define VK_SWAP_CHAIN_H

#include "engine/prehistoric/common/framework/swapchain/Swapchain.h"
#include "engine/prehistoric/core/util/Includes.hpp"

#include "glfw3.h"
#include "vulkan/vulkan.h"

#include "engine/platform/vulkan/framework/surface/VKSurface.h"

class VKSwapchain : public Swapchain
{
public:
	void SetupSwapchain(void* physicalDevice, void* device);

	void SwapBuffers(Window* window) const override;

	void SetVSync(bool vSync) const override;
	void SetFullscreen(Window* window, bool fullscreen) const override;

	void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha) override;
	void ClearScreen() override;

	void DeleteSwapchain(void* device) override;

	VkSwapchainKHR& GetSwapchain() { return swapchain; }
	std::vector<VkImage> GetSwapchainImages() const { return swapchainImages; }
	VkFormat GetSwapchainImageFormat() const { return swapchainImageFormat; }
	VkExtent2D GetSwapchainExtent() const { return swapchainExtent; }

	void RegisterSurface(VKSurface* surface) { this->surface = surface; }
private:
	VKSurface* surface;

	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapchainImages;
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;

	std::vector<VkImageView> swapChainImageViews;
};

#endif
