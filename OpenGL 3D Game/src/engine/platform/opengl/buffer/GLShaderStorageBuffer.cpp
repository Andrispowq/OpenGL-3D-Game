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
*/
void GLShaderStorageBuffer::Store(void* data, const Layout& layout)
{
	this->data = data;
	this->layout = layout;

	glGenBuffers(1, &id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);

	glBufferData(GL_SHADER_STORAGE_BUFFER, layout.getSize(), data, GL_STATIC_DRAW);

	//glBufferData(GL_SHADER_STORAGE_BUFFER, layout.getSize(), nullptr, GL_STATIC_DRAW);

	/*size_t totalOffset = 0;
	for (size_t i = 0; i < layout.memberCount; i++)
	{
		LayoutMember member = layout.members[i];

		size_t offset;
		void* localData;

		switch (member.type)
		{
		case LayoutType::UNSIGNED_INT:
			offset = sizeof(uint32_t) * member.count;
			localData = new uint32_t[member.count];
			memcpy(localData, ((uint32_t*)data + totalOffset), offset);

			break;
		case LayoutType::SIGNED_INT:
			offset = sizeof(int32_t) * member.count;
			localData = new int32_t[member.count];
			memcpy(localData, ((int32_t*)data + totalOffset), offset);

			break;
		case LayoutType::FLOAT:
			offset = sizeof(float) * member.count;
			localData = new float[member.count];
			memcpy(localData, ((float*)data + totalOffset), offset);

			break;
		case LayoutType::DOUBLE:
			offset = sizeof(double) * member.count;
			localData = new double[member.count];
			memcpy(localData, ((double*)data + totalOffset), offset);

			break;
		case LayoutType::VECTOR2F:
			offset = Vector2f::size() * member.count;
			localData = new Vector2f[member.count];
			memcpy(localData, ((Vector2f*)data + totalOffset), offset);

			break;
		case LayoutType::VECTOR3F:
			offset = Vector3f::size() * member.count;
			localData = new Vector3f[member.count];
			memcpy(localData, ((Vector3f*)data + totalOffset), offset);

			break;
		case LayoutType::VECTOR4F:
			offset = Vector4f::size() * member.count;
			localData = new Vector4f[member.count];
			memcpy(localData, ((Vector4f*)data + totalOffset), offset);

			break;
		default:
			break;
		}

		glBufferSubData(GL_SHADER_STORAGE_BUFFER, totalOffset, offset, localData);

		totalOffset += offset;
	}*/

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
