#include "Context.h"
#include "engine/config/FrameworkConfig.h"
#include "engine/platform/windows/opengl/framework/context/GLContext.h"
#include "engine/platform/windows/vulkan/framework/context/VkContext.h"

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
	}

	return *context;
}

Context::~Context()
{
	delete context;
}