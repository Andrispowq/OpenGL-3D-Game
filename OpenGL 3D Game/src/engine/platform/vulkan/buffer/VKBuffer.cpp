#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKBuffer.h"

VKBuffer::VKBuffer(VKPhysicalDevice* physicalDevice, VKDevice* device, size_t size, VkBufferUsageFlags buFlags, VkMemoryPropertyFlags mpFlags)
	: physicalDevice(physicalDevice), device(device), size(size), _buFlags(buFlags), _mpFlags(mpFlags)
{
	VKUtil::CreateBuffer(physicalDevice->getPhysicalDevice(), device->getDevice(), size, buFlags, mpFlags, buffer, memory);
}

VKBuffer::~VKBuffer()
{
	vkDestroyBuffer(device->getDevice(), buffer, nullptr);
	vkFreeMemory(device->getDevice(), memory, nullptr);
}

void VKBuffer::MapMemory(void* data_to_map)
{
	vkMapMemory(device->getDevice(), memory, 0, size, 0, &data);
	memcpy(data, data_to_map, size);
	vkUnmapMemory(device->getDevice(), memory);
}

void VKBuffer::CopyBuffer(VKBuffer* buffer)
{
	VKUtil::CopyBuffer(device, this->buffer, buffer->buffer, size);
}
