#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLComputePipeline.h"

GLComputePipeline::GLComputePipeline(Shader* shader)
	: GLPipeline(shader), ComputePipeline()
{
}

void GLComputePipeline::CreatePipeline(Window* window)
{
}

void GLComputePipeline::BindPipeline() const
{
	GLPipeline::BindPipeline();

	for (const auto& binding : textureBindingTable)
	{
		GLenum access;

		if (binding.second.second == READ_ONLY)
			access = GL_READ_ONLY;
		else if (binding.second.second == WRITE_ONLY)
			access = GL_WRITE_ONLY;
		else if (binding.second.second == READ_WRITE)
			access = GL_READ_WRITE;
		else
			access = GL_READ_WRITE;

		//It is very important to cast the texture to the given type, because Texture has also got an ID and a getID method which gives an explicit renderID, not the internal GL ID
		glBindImageTexture(binding.first, ((GLTexture*) binding.second.first)->getID(), 0, GL_FALSE, 0, access, GL_RGBA32F); //TODO: This is hardcoded here, it should be a texture local field
	}

	for (const auto& ssbo : ssboBindingTable)
	{
		ssbo.second.first->Bind(nullptr, ssbo.first);
	}
}

void GLComputePipeline::RenderPipeline() const
{
	GLPipeline::RenderPipeline();

	glDispatchCompute(invocationSize.x, invocationSize.y, invocationSize.z);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glFinish();
}

void GLComputePipeline::UnbindPipeline() const
{
	for (const auto& ssbo : ssboBindingTable)
	{
		ssbo.second.first->Unbind();
	}

	GLPipeline::UnbindPipeline();
}

void GLComputePipeline::DestroyPipeline()
{
}