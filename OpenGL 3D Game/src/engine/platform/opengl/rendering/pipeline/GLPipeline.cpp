#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLPipeline.h"

GLPipeline::GLPipeline(Shader* shader)
	: Pipeline(shader)
{
}

void GLPipeline::CreatePipeline(Window* window)
{
}

void GLPipeline::BindPipeline() const
{
	shader->Bind(nullptr);
}

void GLPipeline::RenderPipeline() const
{
}

void GLPipeline::UnbindPipeline() const
{
	shader->Unbind();
}

void GLPipeline::DestroyPipeline()
{
}