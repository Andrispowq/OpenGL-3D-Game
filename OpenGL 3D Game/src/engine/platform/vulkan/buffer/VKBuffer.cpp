#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKBuffer.h"

VKBuffer::VKBuffer(VKPhysicalDevice* physicalDevice, VKDevice* device, size_t size, VkBufferUsageFlags buFlags, VkMemoryPropertyFlags mpFlags)
	: physicalDevice(physicalDevice), device(device), size(size), _buFlags(buFlags), _mpFlags(mpFlags)
{
	VKUtil::CreateBuffer(physicalDevice->getPhysicalDevice(), device->getDevice(), size, buFlags, mpFlags, buffer, memory);
}

VKBuffer::VKBuffer(VKBuffer& other)
	: physicalDevice(other.physicalDevice), device(other.device), size(other.size), _buFlags(other._buFlags), _mpFlags(other._mpFlags)
{
	VKUtil::CreateBuffer(physicalDevice->getPhysicalDevice(), device->getDevice(), other.size, other._buFlags, other._mpFlags, buffer, memory);
	VKUtil::CopyBuffer(device, other.buffer, buffer, other.size);
}

VKBuffer::VKBuffer(VKBuffer&& other) noexcept
{
	std::swap(*this, other);
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

VKBuffer& VKBuffer::operator=(VKBuffer other)
{
	std::swap(*this, other);
	return *this;
}
