#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <iostream>
#include <glew.h>
#include <glfw3.h>

#include "engine/config/FrameworkConfig.h"

#include "engine/prehistoric/common/framework/Window.h"

class GLWindow : public Window
{
public:
	GLWindow() : Window(), window(nullptr) {}
	GLWindow(const int& width, const int& height, const char* title, const bool& fullscreen) : Window(width, height, title, fullscreen), window(nullptr) {}

	virtual ~GLWindow();

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