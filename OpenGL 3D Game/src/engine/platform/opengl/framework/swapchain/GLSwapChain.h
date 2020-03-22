#ifndef GL_SWAP_CHAIN_H
#define GL_SWAP_CHAIN_H

#include "engine/prehistoric/common/framework/swapchain/SwapChain.h"

#include "glfw3.h"

class GLSwapChain : public SwapChain
{
public:
	void SetupSwapChain(void* window, void* physicalDevice, void* device);

	void SwapBuffers(void* window) const;

	void SetVSync(bool vSync) const;
	void SetFullscreen(void* window, bool fullscreen) const;

	void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha);
	void ClearScreen();

	void DeleteSwapChain(void* device);
};

#endif