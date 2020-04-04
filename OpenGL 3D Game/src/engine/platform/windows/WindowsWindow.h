#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include "engine/prehistoric/common/framework/Window.h"

#include <glew.h>
#include <glfw3.h>

#include "engine/prehistoric/core/util/loader/TextureLoader.h"
#include "engine/config/FrameworkConfig.h"
#include "engine/platform/Prehistoric.h"

class WindowsWindow : public Window
{
public:
	WindowsWindow() : Window(), window(nullptr) {}
	WindowsWindow(const int& width, const int& height, const char* title, const bool& fullscreen) : Window(width, height, title, fullscreen), window(nullptr) {}

	virtual ~WindowsWindow();

	bool Create() override;
	bool ShouldClose() const override;	
	void Input() override;
	void Render() const override;

	void* GetWindowHandle() const override { return window; }
private:
	bool initGLFW() const;
private:
	GLFWwindow* window;
};

#endif
