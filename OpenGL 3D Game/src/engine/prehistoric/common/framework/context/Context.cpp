#include "engine/prehistoric/core/util/Includes.hpp"
#include "Context.h"
#include "engine/config/FrameworkConfig.h"
#include "engine/platform/opengl/framework/context/GLContext.h"
#include "engine/platform/vulkan/framework/context/VkContext.h"

Context* Context::context = nullptr;

Context& Context::GetContext()
{
	if (context == nullptr)
	{
		if (FrameworkConfig::api == OpenGL)
		{
			context = new GLContext();
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			context = new VkContext();
		}
		else
		{
			PR_LOG_RUNTIME_ERROR("Selected rendering API under res/config/framework.cfg is not supported!");
		}
	}

	return *context;
}

Context::~Context()
{
	//delete context;
}