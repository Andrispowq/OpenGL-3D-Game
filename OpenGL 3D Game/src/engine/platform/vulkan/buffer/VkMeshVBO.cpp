#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VKMeshVBO.h"
#include "engine/platform/vulkan/rendering/pipeline/VKGraphicsPipeline.h"

VKMeshVBO::VKMeshVBO(const Mesh& mesh, void* physicalDevice, void* device)
{
	this->physicalDevice = reinterpret_cast<VKPhysicalDevice*>(physicalDevice);
	this->device = reinterpret_cast<VKDevice*>(device);

	VkCommandPoolCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

	if (vkCreateCommandPool(this->device->GetDevice(), &info, nullptr, &copyCommandPool) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create copy command pool!\n");
	}

	Store(mesh);
}

VKMeshVBO::VKMeshVBO(void* physicalDevice, void* device)
{
	this->physicalDevice = reinterpret_cast<VKPhysicalDevice*>(physicalDevice);
	this->device = reinterpret_cast<VKDevice*>(device);

	VkCommandPoolCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

	if (vkCreateCommandPool(this->device->GetDevice(), &info, nullptr, &copyCommandPool) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create copy command pool!\n");
	}
}

VKMeshVBO::~VKMeshVBO()
{
	vkDestroyCommandPool(device->GetDevice(), copyCommandPool, nullptr);

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

	CreateBuffer(vBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	vkMapMemory(device->GetDevice(), stagingBufferMemory, 0, vBufferSize, 0, &data);
	memcpy(data, dataF, (size_t) vBufferSize);
	vkUnmapMemory(device->GetDevice(), stagingBufferMemory);

	CreateBuffer(vBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

	CopyBuffer(stagingBuffer, vertexBuffer, vBufferSize);

	vkDestroyBuffer(device->GetDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), stagingBufferMemory, nullptr);

	//Creation of index buffer
	CreateBuffer(iBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	vkMapMemory(device->GetDevice(), stagingBufferMemory, 0, iBufferSize, 0, &data);
	memcpy(data, dataS, (size_t) iBufferSize);
	vkUnmapMemory(device->GetDevice(), stagingBufferMemory);

	CreateBuffer(iBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexBuffer, indexBufferMemory);

	CopyBuffer(stagingBuffer, indexBuffer, iBufferSize);

	vkDestroyBuffer(device->GetDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), stagingBufferMemory, nullptr);
}

void VKMeshVBO::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to create buffer!\n");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device->GetDevice(), buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VKUtil::FindMemoryType(memRequirements.memoryTypeBits, physicalDevice->GetPhysicalDevice(), properties);

	if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to allocate buffer memory!\n");
	}

	vkBindBufferMemory(device->GetDevice(), buffer, bufferMemory, 0);
}

void VKMeshVBO::CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size)
{
	VkCommandBufferAllocateInfo copyBufferAI = {};
	copyBufferAI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	copyBufferAI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	copyBufferAI.commandPool = copyCommandPool;
	copyBufferAI.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device->GetDevice(), &copyBufferAI, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;

	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(device->GetGraphicsQueue().GetQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(device->GetGraphicsQueue().GetQueue());

	vkFreeCommandBuffers(device->GetDevice(), copyCommandPool, 1, &commandBuffer);
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

VkVertexInputBindingDescription VKMeshVBO::GetBindingDescription() const
{
    VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = (uint32_t) Vertex::GetSize();
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}  

std::vector<VkVertexInputAttributeDescription> VKMeshVBO::GetAttributeDescriptions() const
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(4);

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = sizeof(float) * 0;// offsetof(Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[1].offset = sizeof(float) * 3;// offsetof(Vertex, texture);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = sizeof(float) * 5;// offsetof(Vertex, normal);

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[3].offset = sizeof(float) * 8;// offsetof(Vertex, normal);

	return attributeDescriptions;
}