#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLGraphicsPipeline.h"

GLGraphicsPipeline::GLGraphicsPipeline(Shader* shader, VertexBuffer* vbo)
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

	if (vbo->getFrontFace() == FrontFace::CLOCKWISE)
	{
		glFrontFace(GL_CW);
	}
	else
	{
		glFrontFace(GL_CCW);
	}

	if (backfaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

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
