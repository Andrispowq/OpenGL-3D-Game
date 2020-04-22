#ifndef VK_MESH_VBO_H
#define VK_MESH_VBO_H

#include "engine/prehistoric/common/buffer/MeshVBO.h"

#include <vulkan/vulkan.h>

#include <vector>

#include "engine/platform/vulkan/rendering/command/VKCommandBuffer.h"
#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"

class VKMeshVBO : public MeshVBO
{
public:
	VKMeshVBO(const Mesh& mesh, Window* window);
	VKMeshVBO(Window* window);

	virtual ~VKMeshVBO() override;

	void Store(const Mesh& mesh) override;

	void Bind(void* commandBuffer) const override;
	void Draw(void* commandBuffer) const override;
	void Unbind() const override;

	VkVertexInputBindingDescription* GetBindingDescription() const;
	std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() const;
private:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkCommandPool copyCommandPool;

	VKPhysicalDevice* physicalDevice;
	VKDevice* device;
	VKSwapchain* swapchain;
};

#endif