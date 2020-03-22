#ifndef VK_SWAP_CHAIN_H
#define VK_SWAP_CHAIN_H

#include "engine/prehistoric/common/framework/swapchain/SwapChain.h"
#include "engine/prehistoric/core/util/Includes.hpp"

#include "glfw3.h"
#include "vulkan/vulkan.h"

class VkSwapChain : public SwapChain
{
public:
	void SetupSwapChain(void* window, void* physicalDevice, void* device);

	void SwapBuffers(void* window) const;

	void SetVSync(bool vSync) const;
	void SetFullscreen(void* window, bool fullscreen) const;

	void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha);
	void ClearScreen();

	void DeleteSwapChain(void* device);

	void SetSurface(VkSurfaceKHR* surface) { this->surface = surface; }
private:
	VkSurfaceKHR* surface; //This is stored here for easier acccess

	VkSwapchainKHR swapChain;
};

#endif
