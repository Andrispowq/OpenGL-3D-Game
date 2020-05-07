#ifndef GL_GRAPHICS_PIPELINE_H
#define GL_GRAPHICS_PIPELINE_H

#include "engine/prehistoric/common/rendering/pipeline/GraphicsPipeline.h"
#include "GLPipeline.h"

#include <glew.h>

class GLGraphicsPipeline : public GLPipeline, public GraphicsPipeline
{
public:
	GLGraphicsPipeline(Shader* shader, VBO* vbo);
	virtual ~GLGraphicsPipeline() {}

	virtual void CreatePipeline(Window* window) override;

	virtual void BindPipeline() const override;
	virtual void RenderPipeline() const override;
	virtual void UnbindPipeline() const override;

	virtual void DestroyPipeline() override;

	virtual bool operator==(const Pipeline& other) override
	{
		if (vbo->operator==(*reinterpret_cast<const GLGraphicsPipeline*>(&other)->getVbo()) && shader->operator==(*reinterpret_cast<const GLPipeline*>(&other)->getShader()))
			return true;

		return false;
	}
};

#endif