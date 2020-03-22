#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

class SwapChain
{
public:
	virtual void SetupSwapChain(void* window, void* physicalDevice, void* device) = 0;

	virtual void SwapBuffers(void* window) const = 0;

	virtual void SetVSync(bool vSync) const = 0;
	virtual void SetFullscreen(void* window, bool fullscreen) const = 0;

	virtual void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha) = 0;
	virtual void ClearScreen() = 0;

	virtual void DeleteSwapChain(void* device) = 0;
};

#endif