#ifndef VK_WINDOW_H
#define VK_WINDOW_H

#include "glfw3.h"

#include "engine/prehistoric/common/framework/Window.h"

class VkWindow : public Window
{
public:
	VkWindow() : Window(), window(nullptr) {}
	VkWindow(const int& width, const int& height, const char* title, const bool& fullscreen) : Window(width, height, title, fullscreen), window(nullptr) {}

	virtual ~VkWindow();

	bool Create() override;

	bool ShouldClose() const override;
	void ClearColor(const float& red, const float& green, const float& blue, const float& alpha) const override;

	void Input() override;
	void ClearScreen() const override;
	void Render() const override;

	inline GLFWwindow* getWindow() const { return window; }
private:
	bool initGLFW() const;
private:
	GLFWwindow* window;
};

#endif