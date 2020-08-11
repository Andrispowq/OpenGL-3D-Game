#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLGraphicsPipeline.h"

GLGraphicsPipeline::GLGraphicsPipeline(Window* window, AssetManager* manager, size_t shaderID, size_t vboID)
	: GLPipeline(window, manager, shaderID), GraphicsPipeline(manager, vboID)
{
}

void GLGraphicsPipeline::BindPipeline() const
{
	GLPipeline::BindPipeline();

	getVertexBuffer()->Bind(nullptr);
}

void GLGraphicsPipeline::RenderPipeline() const
{
	GLPipeline::RenderPipeline();

	VertexBuffer* buff = getVertexBuffer();

	if (buff->getFrontFace() == FrontFace::CLOCKWISE)
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

	buff->Draw(nullptr);
}

void GLGraphicsPipeline::UnbindPipeline() const
{
	getVertexBuffer()->Unbind();

	GLPipeline::UnbindPipeline();
}
