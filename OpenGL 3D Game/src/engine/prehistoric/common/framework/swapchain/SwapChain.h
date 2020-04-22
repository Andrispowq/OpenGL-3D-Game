#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

class Window;

class Swapchain
{
public:
	virtual void SetupSwapchain(Window* window) = 0;

	virtual void SwapBuffers() = 0;

	virtual void SetVSync(bool vSync) const = 0;
	virtual void SetWindowSize(uint32_t width, uint32_t height) = 0;

	virtual void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha) = 0;
	virtual void ClearScreen() = 0;

	virtual void DeleteSwapchain(void* device) = 0;

	virtual uint32_t GetAquiredImageIndex() const { return -1; };
	virtual void* GetDrawCommandBuffer() const { return nullptr; };
protected:
	Window* window;
};

#endif