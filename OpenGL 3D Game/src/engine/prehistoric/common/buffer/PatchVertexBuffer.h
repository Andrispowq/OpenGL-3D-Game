#ifndef PATCH_VERTEX_BUFFER_H
#define PATCH_VERTEX_BUFFER_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "VertexBuffer.h"

class PatchVertexBuffer : public VertexBuffer
{
public:
	PatchVertexBuffer() {}
	virtual ~PatchVertexBuffer() override = 0;

	virtual void Store(const std::vector<Vector2f>& vertices) = 0;

	virtual void Bind(void* commandBuffer) const override = 0;
	virtual void Draw(void* commandBuffer) const = 0;
	virtual void Unbind() const override = 0;
};

#endif