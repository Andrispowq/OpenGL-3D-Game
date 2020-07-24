#ifndef GL_GRAPHICS_PIPELINE_H
#define GL_GRAPHICS_PIPELINE_H

#include "engine/prehistoric/common/rendering/pipeline/GraphicsPipeline.h"
#include "GLPipeline.h"

#include <glew.h>

class GLGraphicsPipeline : public GLPipeline, public GraphicsPipeline
{
public:
	GLGraphicsPipeline(Shader* shader, VertexBuffer* vbo);
	virtual ~GLGraphicsPipeline() {}

	virtual void CreatePipeline(Window* window) override;

	virtual void BindPipeline() const override;
	virtual void RenderPipeline() const override;
	virtual void UnbindPipeline() const override;

	virtual void DestroyPipeline() override;
};

#endif