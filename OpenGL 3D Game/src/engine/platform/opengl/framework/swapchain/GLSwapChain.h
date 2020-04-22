#ifndef GL_SWAP_CHAIN_H
#define GL_SWAP_CHAIN_H

#include "engine/prehistoric/common/framework/swapchain/SwapChain.h"

#include <glew.h>
#include <glfw3.h>

class GLSwapchain : public Swapchain
{
public:
	virtual void SetupSwapchain(Window* window) override;

	virtual void SwapBuffers() override;

	virtual void SetVSync(bool vSync) const override;
	virtual void SetWindowSize(uint32_t width, uint32_t height) override;

	virtual void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha) override;
	virtual void ClearScreen() override;

	virtual void DeleteSwapchain(void* device) override;
};

#endif