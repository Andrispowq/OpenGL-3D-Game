#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKCommandBuffer.h"
#include "VKCommandPool.h"

VKCommandBuffer::VKCommandBuffer(VkDevice& device, VKCommandPool& commandPool)
{
	this->commandPool = &commandPool;
	this->device = &device;

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool.GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to allocate command buffers!\n");
	}
}

void VKCommandBuffer::BindBuffer() const
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to begin command buffer!\n");
	}
	_putenv("");
}

void VKCommandBuffer::UnbindBuffer() const
{
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to end command buffer!\n");
	}
}

void VKCommandBuffer::DeleteBuffer()
{
	vkFreeCommandBuffers(*device, commandPool->GetCommandPool(), 1, &commandBuffer);
}

VKCommandBuffer::~VKCommandBuffer()
{
}