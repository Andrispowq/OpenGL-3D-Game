#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKCommandPool.h"

VKCommandPool::VKCommandPool(VKSurface& surface, VkPhysicalDevice& physicalDevice, VkDevice& device)
	: physicalDevice(physicalDevice), device(device)
{
	this->surface = &surface;

	QueueFamilyIndices indices = VKUtil::FindQueueFamilies(surface.GetSurface(), physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = indices.graphicsFamily;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create command pool!\n");
	}
}

VKCommandPool::~VKCommandPool()
{
	for (auto buffer : buffers)
	{
		delete buffer;
	}

	buffers.clear();

	vkDestroyCommandPool(device, commandPool, nullptr);
}


void VKCommandPool::AddCommandBuffer(VKCommandBuffer& buffer)
{
	buffers.push_back(&buffer);
}

void VKCommandPool::AddCommandBuffer()
{
	VKCommandBuffer* buffer = new VKCommandBuffer(device, *this);

	buffers.push_back(buffer);
}

void VKCommandPool::DeleteCommandBuffer(int index)
{
	VKCommandBuffer* buffer = buffers[index];

	//We erase the element first because accessing deleted memory is not too nice
	auto bufferIndex = std::find(buffers.begin(), buffers.end(), buffer);
	buffers.erase(bufferIndex);

	buffer->DeleteBuffer();
	delete buffer;
}

void VKCommandPool::DeleteCommandBuffers()
{
	VkCommandBuffer* buffs = new VkCommandBuffer[buffers.size()];

	for (size_t i = 0; i < buffers.size(); i++)
	{
		buffs[i] = buffers[i]->GetCommandBuffer();
	}

	vkFreeCommandBuffers(device, commandPool, (uint32_t) buffers.size(), buffs);
	buffers.clear();
	delete[] buffs;
}

void VKCommandPool::BindCommandBuffer(int index) const
{
	buffers[index]->BindBuffer();
}

void VKCommandPool::BindCommandPool() const
{
	for (auto buffer : buffers)
	{
		buffer->BindBuffer();
	}
}

void VKCommandPool::UnbindCommandBuffer(int index) const
{
	buffers[index]->UnbindBuffer();
}

void VKCommandPool::UnbindCommandPool() const
{
	for (auto buffer : buffers)
	{
		buffer->UnbindBuffer();
	}
}