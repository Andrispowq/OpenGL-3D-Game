#include "engine/prehistoric/core/util/Includes.hpp"
#include "DeviceProperties.h"
#include "engine/config/FrameworkConfig.h"
#include "engine/platform/opengl/util/GLDeviceProperties.h"
#include "engine/platform/vulkan/util/VkDeviceProperties.h"

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
			instance = new VkDeviceProperties();
		}
	}

	return *instance;
}

void DeviceProperties::DeleteInstance()
{
	delete instance;
}