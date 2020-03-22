#ifndef VK_QUEUE_MANAGER_H
#define VK_QUEUE_MANAGER_H

#include "vulkan/vulkan.h"

class VkQueueManager
{
public:
	void SetupQueue(VkDevice device, uint32_t family);

	VkQueue GetQueue() const { return queue; }
private:
	VkQueue queue;
};

#endif;