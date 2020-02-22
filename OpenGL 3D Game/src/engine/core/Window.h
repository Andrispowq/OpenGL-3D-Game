#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INDLUDED

#include <iostream>
#include <glew.h>
#include <glfw3.h>

#include "engine/core/Constants.h"

class Window
{
public:
	Window();
	Window(const int& width, const int& height, const char* title, const bool& fullscreen) : width(width), height(height), title(title), fullscreen(fullscreen) {}

	virtual ~Window();

	bool create();
	bool shouldClose() const;
	void clearColor(const float& red, const float& green, const float& blue, const float& alpha) const;
	void input();
	void clearScreen() const;
	void render() const;

	inline GLFWwindow* getWindow() const { return window; }
private:
	bool initGLFW() const;
	bool initGLEW() const;
private:
	int width;
	int height;
	const char* title;
	bool fullscreen;

	GLFWwindow* window;
};

#endif