#ifndef VertexBuffer_H
#define VertexBuffer_H

#include "engine/prehistoric/core/model/Mesh.h"

enum class FrontFace
{
	CLOCKWISE, COUNTER_CLOCKWISE, DOUBLE_SIDED
};

class VertexBuffer
{
public:
	VertexBuffer() : size(0), indexed(false), frontFace(FrontFace::COUNTER_CLOCKWISE) {}
	virtual ~VertexBuffer() = 0;

	virtual void Store(const Mesh& mesh) {}

	virtual void Bind(void* commandBuffer) const = 0;
	virtual void Draw(void* commandBuffer) const = 0;
	virtual void Unbind() const = 0;

	virtual bool operator==(const VertexBuffer& other) = 0;

	FrontFace getFrontFace() const { return frontFace; }
	void SetFrontFace(FrontFace frontFace) { this->frontFace = frontFace; }
protected:
	uint32_t size;
	bool indexed;

	FrontFace frontFace;
};

#endif