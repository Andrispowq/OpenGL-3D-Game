#ifndef VBO_H
#define VBO_H

#include "engine/prehistoric/core/model/Mesh.h"

class VBO
{
public:
	VBO() {}
	virtual ~VBO() = 0;

	virtual void Store(const Mesh& mesh) {}

	virtual void Bind(void* commandBuffer) const = 0;
	virtual void Draw(void* commandBuffer) const = 0;
	virtual void Unbind() const = 0;
protected:
	uint32_t size;
	bool indexed;
};

#endif