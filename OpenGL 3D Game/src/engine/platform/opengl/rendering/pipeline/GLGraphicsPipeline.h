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
		if ((*vbo) == *reinterpret_cast<const GLGraphicsPipeline*>(&other)->vbo && (*shader) == *reinterpret_cast<const GLGraphicsPipeline*>(&other)->shader)
			return true;

		return false;
	}
private:
	VBO* vbo;
};

#endif