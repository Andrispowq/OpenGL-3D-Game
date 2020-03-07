#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

#include "engine/prehistoric/common/framework/context/Context.h"

class GLContext : public Context
{
public:
	bool InitContext() override;
	bool DeleteContext() override;
};

#endif