#ifndef GL_PIPELINE_H
#define GL_PIPELINE_H

#include <glew.h>

#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/framework/Window.h"

class GLPipeline : public Pipeline
{
public:
	GLPipeline(AssetManager* manager, size_t shaderID);
	virtual ~GLPipeline() {}

	void CreatePipeline(Window* window) override;

	virtual void BindPipeline() const override;
	virtual void RenderPipeline() const override;
	virtual void UnbindPipeline() const override;

	void DestroyPipeline() override;
};

#endif