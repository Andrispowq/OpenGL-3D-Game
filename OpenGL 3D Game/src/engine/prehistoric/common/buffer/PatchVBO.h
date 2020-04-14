#ifndef PATCH_VBO_H
#define PATCH_VBO_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "VBO.h"

class PatchVBO : public VBO
{
public:
	PatchVBO() {}
	virtual ~PatchVBO() override = 0;

	virtual void Store(const std::vector<Vector2f>& vertices) = 0;

	virtual void Bind(void* commandBuffer, void* graphicsPipeline) const override = 0;
	virtual void Draw(void* commandBuffer) const = 0;
	virtual void Unbind() const override = 0;
};

#endif