#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkContext.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"

//Context creation and other related stuff
bool VKContext::InitContext(Window* window)
{
	instance.CreateInstance();
	surface.CreateSurface(window, &instance);

	physicalDevice.PickPhysicalDevice(&surface, &instance);
	logicalDevice.CreateLogicalDevice(&surface, &physicalDevice, instance.GetValidationLayers(), physicalDevice.GetDeviceExtensions());
	                                            
	/*
		Some vulkan objects require data from another low-level abstracted vulkan object, like the VKSurface is needed in the VKSwapchain.
		These objects are passed as pointers into that class, so they have access to them. These funcions are managed by this VKContext class, and
		all funcions start with the name Register to distinguish them from the class' members' setters
	*/
	((VKSwapchain*) window->GetSwapchain())->RegisterSurface(&surface);

	return true;
}

bool VKContext::DeleteContext(Window* window)
{
	logicalDevice.DestroyLogicalDevice();

	surface.DeleteSurface(&instance);
	instance.DeleteInstance();

	return true;
}