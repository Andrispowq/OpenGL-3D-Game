#include "engine/prehistoric/core/util/Includes.hpp"
#include "InitDevice.h"

InitDevice* InitDevice::instance;

InitDevice& InitDevice::GetInstance()
{
	if (instance == nullptr)
	{
		if (FrameworkConfig::api == OpenGL)
		{
			PR_LOG_WARNING("CREATE OPENGL DEVICE PROPERTIES!!!!!!!!!");
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			PR_LOG_WARNING("CREATE VULKAN DEVICE PROPERTIES!!!!!!!!!");
		}
	}

	return *instance;
}