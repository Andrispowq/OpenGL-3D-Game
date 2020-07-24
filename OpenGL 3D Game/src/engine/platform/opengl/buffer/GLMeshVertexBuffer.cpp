#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLMeshVertexBuffer.h"

GLMeshVertexBuffer::GLMeshVertexBuffer(const Mesh& mesh) : MeshVertexBuffer()
{
	Store(mesh);
}

GLMeshVertexBuffer::~GLMeshVertexBuffer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void GLMeshVertexBuffer::Store(const Mesh& mesh)
{
	size = static_cast<uint32_t>(mesh.getIndices().size());

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	float* vdata = mesh.GetVertexData();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * Vertex::GetSize(), vdata, GL_STATIC_DRAW);

	uint16_t* idata = mesh.GetIndexData();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices().size() * sizeof(uint16_t), idata, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, int(sizeof(float) * Vertex::GetNumberOfFloats()), (void*) (sizeof(float) * 0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, int(sizeof(float) * Vertex::GetNumberOfFloats()), (void*) (sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, int(sizeof(float) * Vertex::GetNumberOfFloats()), (void*) (sizeof(float) * 5));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, int(sizeof(float) * Vertex::GetNumberOfFloats()), (void*) (sizeof(float) * 8));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLMeshVertexBuffer::Bind(void* commandBuffer) const
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void GLMeshVertexBuffer::Draw(void* commandBuffer) const
{
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, (void*) 0);
}

void GLMeshVertexBuffer::Unbind() const
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}