#include "engine/prehistoric/core/util/Includes.hpp"
#include "DeviceProperties.h"
#include "engine/config/FrameworkConfig.h"
#include "engine/platform/opengl/util/GLDeviceProperties.h"

DeviceProperties* DeviceProperties::instance = nullptr;

DeviceProperties& DeviceProperties::GetInstance()
{
	if (instance == nullptr)
	{
		if (FrameworkConfig::api == OpenGL)
		{
			instance = new GLDeviceProperties();
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			PR_LOG_WARNING("CREATE VULKAN DEVICE PROPERTIES!!!!!!!!!");
		}
	}

	return *instance;
}

void DeviceProperties::DeleteInstance()
{
	delete instance;
}