#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <glew.h>
#include <glfw3.h>

#include "engine/core/util/Constants.h"
#include "engine/config/FrameworkConfig.h"

class Window
{
public:
	Window();
	Window(const int& width, const int& height, const char* title, const bool& fullscreen) : width(width), height(height), title(title), fullscreen(fullscreen) {}

	virtual ~Window();

	bool create();
	bool shouldClose() const;
	void clearColor(const float& red, const float& green, const float& blue, const float& alpha) const;
	void Input();
	void clearScreen() const;
	void Render() const;

	inline GLFWwindow* getWindow() const { return window; }

	inline bool GetClosed() const { return closed; }
	inline void SetClosed(const bool& closed) { this->closed = closed; }
private:
	bool initGLFW() const;
	bool initGLEW() const;
private:
	int width;
	int height;
	const char* title;
	bool fullscreen;
	bool closed;

	GLFWwindow* window;
};

#endif