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

	/*
		Some Vulkan objects require data from another low-level abstracted Vulkan object, like the VKSurface is needed in the VKSwapchain.
		These objects are passed as pointers into that class, so they have access to them. These funcions are managed by this VKContext class, and
		all funcions start with the name Register to distinguish them from the class' members' setters
		This only applies to objects that have an API and Platform independent superclass defined, and cannot take extra Vulkan parameters.
	*/
	static_cast<VKSwapchain*>(window->GetSwapchain())->RegisterSurface(&surface);
	static_cast<VKSwapchain*>(window->GetSwapchain())->RegisterDevice(&logicalDevice);

	return true;
}

bool VKContext::DeleteContext(Window* window)
{
	VKUtil::CleanUp(logicalDevice.GetDevice());

	logicalDevice.DestroyLogicalDevice();

	surface.DeleteSurface(&instance);
	instance.DeleteInstance();

	return true;
}