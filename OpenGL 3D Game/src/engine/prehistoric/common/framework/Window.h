#ifndef WINDOW_H
#define WINDOW_H

#include "engine/prehistoric/core/util/Constants.h"
#include "context/Context.h"

class Window
{
public:
	Window(const int& width, const int& height, const char* title, const bool& fullscreen) : width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT), title(DEFAULT_TITLE), fullscreen(DEFAULT_FULLSCREEN), closed(true) {}
	Window() : width(0), height(0), title("Untitled"), fullscreen(false), closed(true) {}
	
	virtual ~Window() = 0;

	virtual bool Create() = 0;
	virtual bool ShouldClose() const = 0;
	virtual void ClearColor(const float& red, const float& green, const float& blue, const float& alpha) const = 0;
	virtual void Input() = 0;
	virtual void ClearScreen() const = 0;
	virtual void Render() const = 0;

	inline bool GetClosed() const { return closed; }
	inline void SetClosed(const bool& closed) { this->closed = closed; }
protected:
	int width;
	int height;
	const char* title;
	bool fullscreen;
	bool closed;
};

#endif