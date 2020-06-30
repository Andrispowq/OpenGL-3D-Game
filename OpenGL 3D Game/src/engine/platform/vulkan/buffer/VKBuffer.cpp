#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKBuffer.h"

VKBuffer::VKBuffer(VKPhysicalDevice* physicalDevice, VKDevice* device, size_t size, VkBufferUsageFlags buFlags, VkMemoryPropertyFlags mpFlags)
	: physicalDevice(physicalDevice), device(device), size(size), _buFlags(buFlags), _mpFlags(mpFlags)
{
	VKUtil::CreateBuffer(physicalDevice->GetPhysicalDevice(), device->GetDevice(), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer, memory);
}

VKBuffer::~VKBuffer()
{
	vkDestroyBuffer(device->GetDevice(), buffer, nullptr);
	vkFreeMemory(device->GetDevice(), memory, nullptr);
}

void VKBuffer::MapMemory(void* data_to_map)
{
	vkMapMemory(device->GetDevice(), memory, 0, size, 0, &data);
	memcpy(data, data_to_map, size);
	vkUnmapMemory(device->GetDevice(), memory);
}

void VKBuffer::CopyBuffer(VKBuffer* buffer)
{
	VKUtil::CopyBuffer(*device, this->buffer, buffer->buffer, size);
}
