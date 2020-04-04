#ifndef VK_COMMAND_POOL_H
#define VK_COMMAND_POOL_H

#include <vulkan/vulkan.h>

#include "VKCommandBuffer.h"
#include "engine/platform/vulkan/util/VKUtil.h"

class VKCommandPool
{
public:
	VKCommandPool(VKSurface& surface, VkPhysicalDevice& physicalDevice, VkDevice& device);
	virtual ~VKCommandPool();

	void AddCommandBuffer(VKCommandBuffer& buffer);
	void AddCommandBuffer();

	void DeleteCommandBuffer(int index);
	void DeleteCommandBuffers();

	void BindCommandBuffer(int index) const;
	void BindCommandPool() const;

	void UnbindCommandBuffer(int index) const;
	void UnbindCommandPool() const;

	VkCommandPool GetCommandPool() const { return commandPool; }

	VKCommandBuffer* GetCommandBuffer(int index) const { return buffers[index]; }
	std::vector<VKCommandBuffer*> GetCommandBuffers() const { return buffers; }
private:
	VKSurface* surface;
	VkPhysicalDevice* physicalDevice;
	VkDevice* device;

	VkCommandPool commandPool;
	std::vector<VKCommandBuffer*> buffers;
};

#endif