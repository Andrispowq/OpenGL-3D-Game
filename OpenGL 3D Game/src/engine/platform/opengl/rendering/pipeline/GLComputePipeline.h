#ifndef GL_COMPUTE_PIPELINE
#define GL_COMPUTE_PIPELINE

#include "engine/prehistoric/common/rendering/pipeline/ComputePipeline.h"
#include "GLPipeline.h"

#include <glew.h>

#include "engine/platform/opengl/texture/GLTexture.h"

class GLComputePipeline : public GLPipeline, public ComputePipeline
{
public:
	GLComputePipeline(AssetManager* manager, size_t shaderID);
	virtual ~GLComputePipeline() {}

	virtual void CreatePipeline(Window* window) override;

	virtual void BindPipeline() const override;
	virtual void RenderPipeline() const override;
	virtual void UnbindPipeline() const override;

	virtual void DestroyPipeline() override;
};

#endif