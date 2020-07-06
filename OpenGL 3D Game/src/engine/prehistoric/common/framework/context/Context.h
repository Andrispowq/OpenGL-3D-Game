#ifndef CONTEXT_H
#define CONTEXT_H

class Window;

class Context
{
public:
	Context(Window* window) {}
	virtual ~Context() = 0;

	virtual void* GetNativePhysicalDevice() { return nullptr; };
	virtual void* GetNativeDevice() { return nullptr; };

	virtual void* GetPhysicalDevice() { return nullptr; };
	virtual void* GetDevice() { return nullptr; };

protected:
	Window* window;
};

#endif