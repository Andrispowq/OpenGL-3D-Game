#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLGraphicsPipeline.h"

GLGraphicsPipeline::GLGraphicsPipeline(Shader* shader, VBO* vbo)
	: GLPipeline(shader), GraphicsPipeline(vbo)
{
}

void GLGraphicsPipeline::CreatePipeline(Window* window)
{
	GLPipeline::CreatePipeline(window);
}

void GLGraphicsPipeline::BindPipeline() const
{
	GLPipeline::BindPipeline();

	vbo->Bind(nullptr);
}

void GLGraphicsPipeline::RenderPipeline() const
{
	GLPipeline::RenderPipeline();

	if (frontFace == FrontFace::CLOCKWISE)
		glFrontFace(GL_FRONT);
	else
		glFrontFace(GL_BACK);

	vbo->Draw(nullptr);
}

void GLGraphicsPipeline::UnbindPipeline() const
{
	vbo->Unbind();

	GLPipeline::UnbindPipeline();
}

void GLGraphicsPipeline::DestroyPipeline()
{
	GLPipeline::DestroyPipeline();
}
