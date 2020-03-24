#ifndef GL_PIPELINE_H
#define GL_PIPELINE_H

#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"

class GLPipeline : public Pipeline
{
public:
	~GLPipeline() {}

	void CreatePipeline() override;

	void BindPipeline() const override;
	void UnbindPipeline() const override;

	void DestroyPipeline() override;
private:

};

#endif