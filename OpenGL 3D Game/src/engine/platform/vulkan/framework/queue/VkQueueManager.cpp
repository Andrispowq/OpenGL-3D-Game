#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkQueueManager.h"

void VkQueueManager::SetupQueue(VkDevice device, uint32_t family)
{
	vkGetDeviceQueue(device, family, 0, &queue);
}