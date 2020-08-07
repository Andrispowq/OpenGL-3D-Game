#ifndef VK_MESH_VERTEX_BUFFER_H
#define VK_MESH_VERTEX_BUFFER_H

#include "engine/prehistoric/common/buffer/MeshVertexBuffer.h"

#include <vulkan/vulkan.h>

#include "VKBuffer.h"

#include <vector>

#include "engine/platform/vulkan/rendering/command/VKCommandBuffer.h"
#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"

class VKMeshVertexBuffer : public MeshVertexBuffer
{
public:
	VKMeshVertexBuffer(const Mesh& mesh, Window* window);
	VKMeshVertexBuffer(Window* window);

	virtual ~VKMeshVertexBuffer() override;

	void Store(const Mesh& mesh) override;

	void Bind(void* commandBuffer) const override;
	void Draw(void* commandBuffer) const override;
	void Unbind() const override;

	VkVertexInputBindingDescription getBindingDescription() const;
	std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() const;
private:
	VKBuffer* vertexBuffer;
	VKBuffer* indexBuffer;

	VKPhysicalDevice* physicalDevice;
	VKDevice* device;
	VKSwapchain* swapchain;
};

#endif