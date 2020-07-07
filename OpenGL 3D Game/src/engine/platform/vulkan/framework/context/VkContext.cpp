#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkContext.h"

VKContext::VKContext(Window* window)
	: Context(window)
{
	surface.CreateSurface(window, &instance);

	physicalDevice.PickPhysicalDevice(&surface, &instance);
	logicalDevice.CreateLogicalDevice(&surface, &physicalDevice, instance.GetValidationLayers(), physicalDevice.GetDeviceExtensions());

	VKUtil::Init(physicalDevice.GetPhysicalDevice(), logicalDevice.GetDevice());
}

VKContext::~VKContext()
{
	VKUtil::CleanUp(logicalDevice.GetDevice());

	logicalDevice.DestroyLogicalDevice();

	surface.DeleteSurface();
}
