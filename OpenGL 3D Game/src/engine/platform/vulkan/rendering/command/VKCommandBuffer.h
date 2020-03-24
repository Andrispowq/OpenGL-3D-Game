#ifndef VK_COMMAND_BUFFER_H
#define VK_COMMAND_BUFFER_H

class VKCommandPool;

#include <vulkan/vulkan.h>

class VKCommandBuffer
{
public:
	VKCommandBuffer(VKCommandPool& commandPool);
	virtual ~VKCommandBuffer();

	VkCommandBuffer& GetCommandBuffer() { return commandBuffer; }
private:
	VKCommandPool& pool;

	VkCommandBuffer commandBuffer;
};

#endif