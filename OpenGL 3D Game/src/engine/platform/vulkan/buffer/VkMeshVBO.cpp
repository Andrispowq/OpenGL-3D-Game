#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VKMeshVBO.h"
#include "engine/platform/vulkan/rendering/pipeline/VKGraphicsPipeline.h"

VKMeshVBO::VKMeshVBO(const Mesh& mesh, void* physicalDevice, void* device)
{
	this->physicalDevice = reinterpret_cast<VKPhysicalDevice*>(physicalDevice);
	this->device = reinterpret_cast<VKDevice*>(device);

	Store(mesh);
}

VKMeshVBO::VKMeshVBO(void* physicalDevice, void* device)
{
	this->physicalDevice = reinterpret_cast<VKPhysicalDevice*>(physicalDevice);
	this->device = reinterpret_cast<VKDevice*>(device);
}

VKMeshVBO::~VKMeshVBO()
{
	vkDestroyBuffer(device->GetDevice(), vertexBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), vertexBufferMemory, nullptr);

	vkDestroyBuffer(device->GetDevice(), indexBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), indexBufferMemory, nullptr);
}

void VKMeshVBO::Store(const Mesh& mesh)
{
	this->size = (uint8_t) mesh.getIndices().size();

	//Building mesh data
	VkDeviceSize vBufferSize = mesh.getVertices().size() * Vertex::GetSize();
	VkDeviceSize iBufferSize = mesh.getIndices().size() * Vertex::GetSize();
	void* data;

	float* dataF = mesh.GetVertexData();
	uint16_t* dataS = mesh.GetIndexData();

	//Creation of the vertex buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VKUtil::CreateBuffer(physicalDevice->GetPhysicalDevice(), device->GetDevice(), vBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	vkMapMemory(device->GetDevice(), stagingBufferMemory, 0, vBufferSize, 0, &data);
	memcpy(data, dataF, (size_t) vBufferSize);
	vkUnmapMemory(device->GetDevice(), stagingBufferMemory);

	VKUtil::CreateBuffer(physicalDevice->GetPhysicalDevice(), device->GetDevice(), vBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

	VKUtil::CopyBuffer(*device, stagingBuffer, vertexBuffer, vBufferSize);

	vkDestroyBuffer(device->GetDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), stagingBufferMemory, nullptr);

	//Creation of index buffer
	VKUtil::CreateBuffer(physicalDevice->GetPhysicalDevice(), device->GetDevice(), iBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	vkMapMemory(device->GetDevice(), stagingBufferMemory, 0, iBufferSize, 0, &data);
	memcpy(data, dataS, (size_t) iBufferSize);
	vkUnmapMemory(device->GetDevice(), stagingBufferMemory);

	VKUtil::CreateBuffer(physicalDevice->GetPhysicalDevice(), device->GetDevice(), iBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexBuffer, indexBufferMemory);

	VKUtil::CopyBuffer(*device, stagingBuffer, indexBuffer, iBufferSize);

	vkDestroyBuffer(device->GetDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), stagingBufferMemory, nullptr);
}

void VKMeshVBO::Bind(void* commandBuffer, void* graphicsPipeline) const
{
	VkCommandBuffer* buffer = reinterpret_cast<VkCommandBuffer*>(commandBuffer);
	vkCmdBindPipeline(*buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *reinterpret_cast<VkPipeline*>(graphicsPipeline));

	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(*buffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(*buffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
}

void VKMeshVBO::Draw(void* commandBuffer) const
{
	vkCmdDrawIndexed(*reinterpret_cast<VkCommandBuffer*>(commandBuffer), size, 1, 0, 0, 0);
}

void VKMeshVBO::Unbind() const
{

}

VkVertexInputBindingDescription* VKMeshVBO::GetBindingDescription() const
{
    VkVertexInputBindingDescription* bindingDescription = new VkVertexInputBindingDescription;
	bindingDescription->binding = 0;
	bindingDescription->stride = (uint32_t) Vertex::GetSize();
	bindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}  

std::vector<VkVertexInputAttributeDescription> VKMeshVBO::GetAttributeDescriptions() const
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(4);

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, texture);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, normal);

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(Vertex, colour);

	return attributeDescriptions;
}