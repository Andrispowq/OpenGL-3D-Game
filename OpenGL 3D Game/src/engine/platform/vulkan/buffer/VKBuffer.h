#ifndef VK_BUFFER_H
#define VK_BUFFER_H

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"

class VKBuffer
{
public:
	VKBuffer(VKPhysicalDevice* physicalDevice, VKDevice* device, size_t size, VkBufferUsageFlags buFlags, VkMemoryPropertyFlags mpFlags);
	virtual ~VKBuffer();

	void MapMemory(void* data_to_map);
	void CopyBuffer(VKBuffer* buffer);

	inline VkBuffer& getBuffer() { return buffer; }
	inline VkDeviceMemory& getMemory() { return memory; }

	inline size_t getSize() const {  return size; }
	inline void setSize(size_t size) { this->size = size; }

	inline void setBufferUsageFlags(VkBufferUsageFlags _buFlags) { this->_buFlags = _buFlags; }
	inline void setMemoryPropertyFlags(VkMemoryPropertyFlags _mpFlags) { this->_mpFlags = _mpFlags; }

	inline void setPhysicalDevice(void* physicalDev) { this->physicalDevice = (VKPhysicalDevice*)physicalDev; }
	inline void setDevice(void* dev) { this->device = (VKDevice*)dev; }

	bool operator==(const VKBuffer& other) { return data == other.data && size == other.size; }
private:
	VkBuffer buffer;
	VkDeviceMemory memory;

	void* data;
	size_t size;

	VkBufferUsageFlags _buFlags;
	VkMemoryPropertyFlags _mpFlags;

	VKPhysicalDevice* physicalDevice;
	VKDevice* device;
};

#endif