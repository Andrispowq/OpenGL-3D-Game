#ifndef VK_COMMAND_POOL_H
#define VK_COMMAND_POOL_H

#include <vulkan/vulkan.h>

#include "VKCommandBuffer.h"
#include "engine/platform/vulkan/util/VKUtil.h"

class VKCommandPool
{
public:
	VKCommandPool();
	virtual ~VKCommandPool();

	void AddCommandBuffer(const VKCommandBuffer& buffer);

	void BindCommandBuffer(int index) const;
	void BindCommandPool() const;

	void UnbindCommandBuffer(int index) const;
	void UnbindCommandPool();

	VKCommandBuffer& GetCommandBuffer(int index) { return buffers[index]; }
	std::vector<VKCommandBuffer>& GetCommandBuffers(int index) { return buffers; }
private:
	VkCommandPool pool;
	std::vector<VKCommandBuffer> buffers;
};

#endif