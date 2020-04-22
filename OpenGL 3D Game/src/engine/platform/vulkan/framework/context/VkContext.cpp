#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkContext.h"

//Context creation and other related stuff
bool VKContext::InitContext(Window* window)
{
	instance.CreateInstance();
	surface.CreateSurface(window, &instance);

	physicalDevice.PickPhysicalDevice(&surface, &instance);
	logicalDevice.CreateLogicalDevice(&surface, &physicalDevice, instance.GetValidationLayers(), physicalDevice.GetDeviceExtensions());
	
	VKUtil::Init(physicalDevice.GetPhysicalDevice(), logicalDevice.GetDevice());

	return true;
}

bool VKContext::DeleteContext(Window* window)
{
	VKUtil::CleanUp(logicalDevice.GetDevice());

	logicalDevice.DestroyLogicalDevice();

	surface.DeleteSurface();
	instance.DeleteInstance();

	return true;
}