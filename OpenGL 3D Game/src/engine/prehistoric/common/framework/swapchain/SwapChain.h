#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

class Window;

class Swapchain
{
public:
	virtual void SetupSwapchain(void* physicalDevice, void* device) = 0;

	virtual void SwapBuffers(Window* window) const = 0;

	virtual void SetVSync(bool vSync) const = 0;
	virtual void SetFullscreen(Window* window, bool fullscreen) const = 0;

	virtual void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha) = 0;
	virtual void ClearScreen() = 0;

	virtual void DeleteSwapchain(void* device) = 0;
};

#endif