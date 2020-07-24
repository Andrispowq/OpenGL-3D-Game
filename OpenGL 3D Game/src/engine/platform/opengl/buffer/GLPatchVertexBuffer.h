#ifndef GL_PATCH_VERTEX_BUFFER_H
#define GL_PATCH_VERTEX_BUFFER_H

#include <glew.h>

#include "engine/prehistoric/common/buffer/PatchVertexBuffer.h"

class GLPatchVertexBuffer : public PatchVertexBuffer
{
public:
	GLPatchVertexBuffer(const std::vector<Vector2f>& vertices);
	GLPatchVertexBuffer() : PatchVertexBuffer(), vao(0), vbo(0) {}

	virtual ~GLPatchVertexBuffer() override;

	void Store(const std::vector<Vector2f>& vertices) override;

	void Bind(void* commandBuffer) const override;
	void Draw(void* commandBuffer) const override;
	void Unbind() const override;

	virtual bool operator==(const VertexBuffer& other) override
	{
		return vao == (*reinterpret_cast<const GLPatchVertexBuffer*>(&other)).vao && vbo == (*reinterpret_cast<const GLPatchVertexBuffer*>(&other)).vbo;
	}
private:
	GLuint vao;
	GLuint vbo;
};

#endif