#ifndef VK_MESH_VBO_H
#define VK_MESH_VBO_H

#include "vulkan/vulkan.h"

#include "engine/prehistoric/common/buffer/MeshVBO.h"

class VKMeshVBO : public MeshVBO
{
public:
	VKMeshVBO(const Mesh& mesh);
	VKMeshVBO() : MeshVBO() {}

	virtual ~VKMeshVBO() override;

	void Store(const Mesh& mesh) override;

	void Bind() const override;
	void Draw() const override;
	void Unbind() const override;

	void SetBuffer(VkCommandBuffer buffer) { this->buffer = buffer; }
private:
	VkCommandBuffer buffer;
};

#endif