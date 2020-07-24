#ifndef GL_MESH_VERTEX_BUFFER_H
#define GL_MESH_VERTEX_BUFFER_H

#include <glew.h>

#include "engine/prehistoric/common/buffer/MeshVertexBuffer.h"

class GLMeshVertexBuffer : public MeshVertexBuffer
{
public:
	GLMeshVertexBuffer(const Mesh& mesh);
	GLMeshVertexBuffer() : MeshVertexBuffer(), vao(0), vbo(0), ibo(0) {}

	virtual ~GLMeshVertexBuffer() override;

	void Store(const Mesh& mesh) override;

	void Bind(void* commandBuffer) const override;
	void Draw(void* commandBuffer) const override;
	void Unbind() const override;

	virtual bool operator==(const VertexBuffer& other) override
	{
		return vao == (*reinterpret_cast<const GLMeshVertexBuffer*>(&other)).vao && vbo == (*reinterpret_cast<const GLMeshVertexBuffer*>(&other)).vbo
			&& ibo == (*reinterpret_cast<const GLMeshVertexBuffer*>(&other)).ibo;
	}
private:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};

#endif