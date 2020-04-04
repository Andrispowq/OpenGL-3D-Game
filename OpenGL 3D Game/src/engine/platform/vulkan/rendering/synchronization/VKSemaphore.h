#ifndef VK_SEMAPHORE_H
#define VK_SEMAPHORE_H

#include <vulkan/vulkan.h>

class VKSemaphore
{
public:
	VKSemaphore(VkDevice& device);
	virtual ~VKSemaphore();

	VkSemaphore& GetSemaphore() { return semaphore; }
private:
	VkDevice* device;

	VkSemaphore semaphore;
};

#endif