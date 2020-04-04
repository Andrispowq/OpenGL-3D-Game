#ifndef VK_MESH_VBO_H
#define VK_MESH_VBO_H

#include "engine/prehistoric/common/buffer/MeshVBO.h"

#include <vulkan/vulkan.h>

#include <vector>

#include "engine/platform/vulkan/rendering/command/VKCommandBuffer.h"
#include "engine/platform/vulkan/framework/device/VKDevice.h"

class VKMeshVBO : public MeshVBO
{
public:
	VKMeshVBO(const Mesh& mesh, void* physicalDevice, void* device);
	VKMeshVBO(void* physicalDevice, void* device);

	virtual ~VKMeshVBO() override;

	void Store(const Mesh& mesh) override;

	void Bind(void* commandBuffer, void* graphicsPipeline) const override;
	void Draw(void* commandBuffer) const override;
	void Unbind() const override;

	void RegisterBuffer(VKCommandBuffer* buffer) { this->buffer = buffer; }

	VkVertexInputBindingDescription GetBindingDescription() const;
	std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() const;
private:
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size);
private:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkCommandPool copyCommandPool;

	uint8_t size;

	VKCommandBuffer* buffer;
	VKDevice* device;
	VKPhysicalDevice* physicalDevice;
};

#endif