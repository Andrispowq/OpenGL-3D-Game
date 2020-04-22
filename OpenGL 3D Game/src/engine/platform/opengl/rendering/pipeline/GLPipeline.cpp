#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLPipeline.h"

GLPipeline::GLPipeline(Shader* shader, VBO* vbo)
	: Pipeline(shader, vbo)
{
}

void GLPipeline::CreatePipeline(Window* window)
{
}

void GLPipeline::BindPipeline() const
{
	vbo->Bind(nullptr);
	shader->Bind(nullptr);
}

void GLPipeline::RenderPipeline() const
{
	vbo->Draw(nullptr);
}

void GLPipeline::UnbindPipeline() const
{
	shader->Unbind();
	vbo->Unbind();
}

void GLPipeline::DestroyPipeline()
{
}