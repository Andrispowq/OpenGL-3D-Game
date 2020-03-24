#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLPipeline.h"

void GLPipeline::CreatePipeline()
{
}

void GLPipeline::BindPipeline() const
{
	shader->Bind();
}

void GLPipeline::UnbindPipeline() const
{
	shader->Unbind();
}

void GLPipeline::DestroyPipeline()
{
}