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

		glBindImageTexture(binding.first, binding.second.first->getID(), 0, GL_FALSE, 0, access, GL_RGBA32F); //TODO: This is hardcoded here, it should be a texture local field
	}
}

void GLComputePipeline::RenderPipeline() const
{
	GLPipeline::RenderPipeline();

	glDispatchCompute(invocationSize.x, invocationSize.y, invocationSize.z);
	glFinish();
}

void GLComputePipeline::UnbindPipeline() const
{
	GLPipeline::UnbindPipeline();
}

void GLComputePipeline::DestroyPipeline()
{
}
