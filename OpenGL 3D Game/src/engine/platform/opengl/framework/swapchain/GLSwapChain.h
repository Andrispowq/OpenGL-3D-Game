#ifndef GL_SWAP_CHAIN_H
#define GL_SWAP_CHAIN_H

#include "engine/prehistoric/common/framework/swapchain/SwapChain.h"

#include <glew.h>
#include <glfw3.h>

class GLSwapChain : public Swapchain
{
public:
	void SetupSwapchain(void* physicalDevice, void* device) override;

	void SwapBuffers(Window* window) const override;

	void SetVSync(bool vSync) const override;
	void SetFullscreen(Window* window, bool fullscreen) const override;

	void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha) override;
	void ClearScreen() override;

	void DeleteSwapchain(void* device) override;
};

#endif