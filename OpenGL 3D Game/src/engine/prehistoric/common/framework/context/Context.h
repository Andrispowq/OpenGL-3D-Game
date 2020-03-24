#ifndef CONTEXT_H
#define CONTEXT_H

class Window;

class Context
{
public:
	Context() {}
	virtual ~Context() = 0;

	virtual bool InitContext(Window* window) = 0;
	virtual bool DeleteContext(Window* window) = 0;

	virtual void* GetPhysicalDevice() { return nullptr; };
	virtual void* GetDevice() { return nullptr; };
};

#endif