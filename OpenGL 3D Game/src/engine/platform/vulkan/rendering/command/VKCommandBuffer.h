#ifndef VK_COMMAND_BUFFER_H
#define VK_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"

class VKCommandPool;

class VKCommandBuffer
{
public:
	VKCommandBuffer(VkDevice device, VKCommandPool& commandPool);
	virtual ~VKCommandBuffer();

	void BindBuffer() const;
	void UnbindBuffer() const;

	void DeleteBuffer();

	VkCommandBuffer& GetCommandBuffer() { return commandBuffer; }
private:
	VKCommandPool* commandPool;
	VkDevice* device;

	VkCommandBuffer commandBuffer;
};

#endif