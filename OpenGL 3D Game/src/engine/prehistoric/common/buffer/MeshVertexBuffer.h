#ifndef MESH_VERTEX_BUFFER_H
#define MESH_VERTEX_BUFFER_H

#include "VertexBuffer.h"

#include "engine/prehistoric/core/model/Mesh.h"

class MeshVertexBuffer : public VertexBuffer
{
public:
	MeshVertexBuffer() {}
	
	virtual ~MeshVertexBuffer() override = 0;

	virtual void Store(const Mesh& mesh) override = 0;

	virtual void Bind(void* commandBuffer) const override  = 0;
	virtual void Draw(void* commandBuffer) const = 0;
	virtual void Unbind() const override = 0;

	virtual bool operator==(const VertexBuffer& other) override = 0;
};

#endif