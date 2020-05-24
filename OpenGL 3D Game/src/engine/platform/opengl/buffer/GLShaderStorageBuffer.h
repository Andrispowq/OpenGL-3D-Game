#ifndef GL_SSBO_H
#define GL_SSBO_H

#include "engine/prehistoric/common/buffer/ShaderStorageBuffer.h"

#include <glew.h>

class GLShaderStorageBuffer : public ShaderStorageBuffer
{
public:
	GLShaderStorageBuffer() : id(0xffffffff) {}
	GLShaderStorageBuffer(void* data, const Layout& layout);

	virtual ~GLShaderStorageBuffer() override;

	virtual void Store(void* data, const Layout& layout) override;

	virtual void Bind(void* commandBuffer, uint32_t binding) const override;
	virtual void Unbind() const override;

	virtual void MapBuffer() override;
	virtual void UnmapBuffer() override;

	virtual bool operator==(const ShaderStorageBuffer& other) override
	{
		if (id == reinterpret_cast<const GLShaderStorageBuffer*>(&other)->id)
			return true;

		return false;
	}
private:
	GLuint id;
};

#endif