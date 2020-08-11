#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkContext.h"

VKContext::VKContext(Window* window)
	: Context(window)
{
	surface.CreateSurface(window, &instance);

	physicalDevice.PickPhysicalDevice(&surface, &instance);
	logicalDevice.CreateLogicalDevice(&physicalDevice, &surface, instance.getValidationLayers());

	VKUtil::Init(physicalDevice.getPhysicalDevice(), logicalDevice.getDevice());
}

VKContext::~VKContext()
{
	VKUtil::CleanUp(logicalDevice.getDevice());

	logicalDevice.DestroyLogicalDevice();
	surface.DeleteSurface();
}
