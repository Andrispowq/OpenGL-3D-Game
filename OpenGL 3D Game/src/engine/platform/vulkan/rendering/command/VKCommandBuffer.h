#ifndef VK_COMMAND_BUFFER_H
#define VK_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"

class VKCommandPool;

class VKCommandBuffer
{
public:
	VKCommandBuffer(VKCommandPool* commandPool, VkDevice device);
	virtual ~VKCommandBuffer() {}

	void BindBuffer() const;
	void UnbindBuffer() const;

	void DeleteBuffer();

	//Needs to return a reference
	VkCommandBuffer& getCommandBuffer() { return commandBuffer; }
private:
	VKCommandPool* commandPool;
	VkDevice device;

	VkCommandBuffer commandBuffer;
};

#endif