#ifndef VK_DEBUG_MESSENGER_H
#define VK_DEBUG_MESSENGER_H

#include <vulkan/vulkan.h>

class VKDebugMessenger
{
public:
	void CreateMessenger(VkInstance& instance);
	void DeleteMessenger(VkInstance& instance);

	static void CreateMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& messengerCreateInfo);

	VkDebugUtilsMessengerEXT GetDebugMessenger() const { return debugMessenger; }
private:
	VkDebugUtilsMessengerEXT debugMessenger;
};

#endif