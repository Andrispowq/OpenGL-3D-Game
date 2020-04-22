#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLPatchVBO.h"

GLPatchVBO::GLPatchVBO(const std::vector<Vector2f>& vertices) : PatchVBO()
{
	Store(vertices);
}

GLPatchVBO::~GLPatchVBO()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void GLPatchVBO::Store(const std::vector<Vector2f>& vertices)
{
	size = vertices.size();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	float* vdata = new float[vertices.size() * 2];

	for (size_t i = 0; i < vertices.size(); i++)
	{
		vdata[i * 2 + 0] = vertices[i].x;
		vdata[i * 2 + 1] = vertices[i].y;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector2f), vdata, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glPatchParameteri(GL_PATCH_VERTICES, size);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLPatchVBO::Bind(void* commandBuffer) const
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
}

void GLPatchVBO::Draw(void* commandBuffer) const
{
	glDrawArrays(GL_PATCHES, 0, size);
}

void GLPatchVBO::Unbind() const
{
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}