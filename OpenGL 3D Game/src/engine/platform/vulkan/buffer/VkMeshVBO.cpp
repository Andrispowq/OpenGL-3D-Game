#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VKMeshVBO.h"
#include "engine/platform/vulkan/rendering/pipeline/VKGraphicsPipeline.h"

VKMeshVBO::VKMeshVBO(const Mesh& mesh, Window* window)
{
	this->physicalDevice = reinterpret_cast<VKPhysicalDevice*>(window->GetContext()->GetPhysicalDevice());
	this->device = reinterpret_cast<VKDevice*>(window->GetContext()->GetDevice());

	this->swapchain = (VKSwapchain*) window->GetSwapchain();

	Store(mesh);
}

VKMeshVBO::VKMeshVBO(Window* window)
{
	this->physicalDevice = reinterpret_cast<VKPhysicalDevice*>(window->GetContext()->GetPhysicalDevice());
	this->device = reinterpret_cast<VKDevice*>(window->GetContext()->GetDevice());

	this->swapchain = (VKSwapchain*)window->GetSwapchain();
}

VKMeshVBO::~VKMeshVBO()
{
	delete vertexBuffer;
	delete indexBuffer;
}

void VKMeshVBO::Store(const Mesh& mesh)
{
	this->size = (uint32_t) mesh.getIndices().size();

	//Building mesh data
	VkDeviceSize vBufferSize = mesh.getVertices().size() * Vertex::GetSize();
	VkDeviceSize iBufferSize = mesh.getIndices().size() * sizeof(uint16_t);

	this->vertexBuffer = new VKBuffer(physicalDevice, device, vBufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	this->indexBuffer = new VKBuffer(physicalDevice, device, iBufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	float* vData = mesh.GetVertexData();
	uint16_t* iData = mesh.GetIndexData();

	//Creation of the vertex buffer
	VKBuffer* stagingBuffer = new VKBuffer(physicalDevice, device, vBufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer->MapMemory(vData);
	stagingBuffer->CopyBuffer(vertexBuffer); //Copies the object which invoked the function to the buffer passed as an argument

	delete stagingBuffer;
	 
	//Creation of index buffer
	stagingBuffer = new VKBuffer(physicalDevice, device, iBufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer->MapMemory(iData);
	stagingBuffer->CopyBuffer(indexBuffer); //Copies the object which invoked the function to the buffer passed as an argument

	delete stagingBuffer;
}

void VKMeshVBO::Bind(void* commandBuffer) const
{
	VKCommandBuffer* buffer = (VKCommandBuffer*)commandBuffer;

	//VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(buffer->GetCommandBuffer(), 0, 1, &vertexBuffer->getBuffer(), offsets);
	vkCmdBindIndexBuffer(buffer->GetCommandBuffer(), indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT16);
}

void VKMeshVBO::Draw(void* commandBuffer) const
{
	vkCmdDrawIndexed(((VKCommandBuffer*) commandBuffer)->GetCommandBuffer(), size, 1, 0, 0, 0);
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
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = 0;

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[1].offset = uint32_t(Vector3f::size());

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = uint32_t(Vector3f::size() + Vector2f::size());

	return attributeDescriptions;
}