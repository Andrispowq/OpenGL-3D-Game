#ifndef VK_DEBUG_MESSENGER_H
#define VK_DEBUG_MESSENGER_H

#include "vulkan/vulkan.h"

class VkDebugMessenger
{
public:
	void CreateMessenger(VkInstance instance);
	void DeleteMessenger(VkInstance instance);
private:
	VkDebugUtilsMessengerEXT debugMessenger;
};

#endif