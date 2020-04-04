#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLPipeline.h"

GLPipeline::GLPipeline(Shader* shader)
{
	this->shader = shader;
}

void GLPipeline::CreatePipeline(Window* window, MeshVBO* vbo)
{
}

void GLPipeline::BindPipeline()
{
	shader->Bind(nullptr);
}

void GLPipeline::UnbindPipeline() const
{
	shader->Unbind();
}

void GLPipeline::DestroyPipeline()
{
}