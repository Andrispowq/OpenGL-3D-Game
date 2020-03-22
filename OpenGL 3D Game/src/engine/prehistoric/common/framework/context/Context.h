#ifndef CONTEXT_H
#define CONTEXT_H

#include "engine/prehistoric/common/framework/Window.h"

class Context
{
public:
	static Context& GetContext();

	virtual bool InitContext(Window* window) = 0;
	virtual bool DeleteContext(Window* window) = 0;
protected:
	Context() {}
	virtual ~Context() = 0;

	static Context* context;
};

#endif