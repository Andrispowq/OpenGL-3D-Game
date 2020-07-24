#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "engine/prehistoric/common/framework/Window.h"

class Framebuffer
{
public:
	Framebuffer(Window* window);
	virtual ~Framebuffer();

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

private:
	Window* window;
};

#endif