#ifndef GL_COMPUTE_PIPELINE
#define GL_COMPUTE_PIPELINE

#include "engine/prehistoric/common/rendering/pipeline/ComputePipeline.h"
#include "GLPipeline.h"

#include <glew.h>

class GLComputePipeline : public GLPipeline, public ComputePipeline
{
public:
	GLComputePipeline(Shader* shader);
	virtual ~GLComputePipeline() {}

	virtual void CreatePipeline(Window* window) override;

	virtual void BindPipeline() const override;
	virtual void RenderPipeline() const override;
	virtual void UnbindPipeline() const override;

	virtual void DestroyPipeline() override;

	virtual bool operator==(const Pipeline& other)
	{
		if (GLPipeline::shader->operator==(*reinterpret_cast<const GLPipeline*>(&other)->getShader()))
			return true;

		return false;
	}
};

#endif