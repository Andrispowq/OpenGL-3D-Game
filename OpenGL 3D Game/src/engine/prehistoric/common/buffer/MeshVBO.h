#ifndef MESH_VBO_H
#define MESH_VBO_H

#include "engine/prehistoric/core/model/Mesh.h"

class MeshVBO
{
public:
	MeshVBO() : size(0) {}
	
	virtual ~MeshVBO() = 0;

	virtual void Store(const Mesh& mesh) = 0;

	virtual void Bind(void* commandBuffer, void* graphicsPipeline) const = 0;
	virtual void Draw(void* commandBuffer) const = 0;
	virtual void Unbind() const = 0;
protected:
	uint32_t size;
};

#endif