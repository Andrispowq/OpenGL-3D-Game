#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLPipeline.h"

GLPipeline::GLPipeline(AssetManager* manager, size_t shaderID)
	: Pipeline(manager, shaderID)
{
}

void GLPipeline::CreatePipeline(Window* window)
{
}

void GLPipeline::BindPipeline() const
{
	getShader()->Bind(nullptr);
}

void GLPipeline::RenderPipeline() const
{
}

void GLPipeline::UnbindPipeline() const
{
	getShader()->Unbind();
}

void GLPipeline::DestroyPipeline()
{
}