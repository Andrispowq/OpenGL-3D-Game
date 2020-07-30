#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLShaderStorageBuffer.h"

GLShaderStorageBuffer::GLShaderStorageBuffer(void* data, const Layout& layout)
{
	Store(data, layout);
}

GLShaderStorageBuffer::~GLShaderStorageBuffer()
{
	glDeleteBuffers(1, &id);
}

/*
	Please note that this function is extremely dangerous, it does not have any type checks, memory bound chacks, or whatever, so you need to specify the
	layout CORRECTLY, so it will not cause a crash or something even worse

	'data' gets managed by OpenGL
*/
void GLShaderStorageBuffer::Store(void* data, const Layout& layout)
{
	this->data = data;
	this->layout = layout;

	glGenBuffers(1, &id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, layout.getSize(), data, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GLShaderStorageBuffer::Bind(void* commandBuffer, uint32_t binding) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, id);
}

void GLShaderStorageBuffer::Unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GLShaderStorageBuffer::MapBuffer()
{
	data = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
}

void GLShaderStorageBuffer::UnmapBuffer()
{
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
