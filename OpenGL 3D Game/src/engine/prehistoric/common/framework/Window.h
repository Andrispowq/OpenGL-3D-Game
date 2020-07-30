#ifndef WINDOW_H
#define WINDOW_H

#include "engine/prehistoric/core/util/Constants.h"

#include "engine/prehistoric/common/framework/swapchain/Swapchain.h"
#include "engine/prehistoric/common/framework/context/Context.h"

#include "engine/config/FrameworkConfig.h"

class Window
{
public:
	Window(uint32_t width, uint32_t height, const char* title, bool fullscreen);
	Window();
	
	virtual ~Window() = 0;

	void SetClearColor(float red, float green, float blue, float alpha) const { swapchain->SetClearColor(red, green, blue, alpha); }
	void ClearScreen() const { swapchain->ClearScreen(); }

	void SetVSync(bool vsync) const { swapchain->SetVSync(vsync); }

	void DeleteSwapChain(void* device) { swapchain->DeleteSwapchain(device); }

	virtual bool Create() = 0;
	virtual bool ShouldClose() const = 0; 
	virtual void Input() = 0;
	virtual void Render() const = 0;

	virtual void SetFullscreen(bool fullscreen) = 0;

	bool getClosed() const { return closed; }
	void setClosed(const bool& closed) { this->closed = closed; }

	uint32_t getWidth() const { return width; }
	uint32_t getHeight() const { return height; }
	bool getResized() const { return resized; }

	Swapchain* getSwapchain() const { return swapchain; }
	Context* getContext() const { return context; }

	virtual void* getWindowHandle() const = 0;

	void setWidth(int width) { this->width = width; FrameworkConfig::windowWidth = width; }
	void setHeight(int height) { this->height = height; FrameworkConfig::windowHeight = height; }
	void setResized(bool resized) { this->resized = resized; }

protected:
	uint32_t width;
	uint32_t height;
	const char* title;
	bool fullscreen;
	bool closed;

	bool resized;

	Swapchain* swapchain;
	Context* context;
};

#endif