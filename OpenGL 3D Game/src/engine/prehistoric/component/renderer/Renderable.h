#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "engine/prehistoric/component/Component.h"

#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/framework/Window.h"
#include "engine/prehistoric/common/rendering/RenderingEngine.h"

#include "engine/prehistoric/core/util/Includes.hpp"

class Renderable : public Component
{
public:
	Renderable(Pipeline* pipeline, Window* window);
	Renderable(Window* window);
	virtual ~Renderable();

	static void CleanUp();

	static void RecreatePipelines();

	virtual void Render(const RenderingEngine& renderingEngine) const = 0;
	virtual void BatchRender(const RenderingEngine& renderingEngine) const = 0;

	static std::vector<Pipeline*> GetPipelines() { return pipelines; }

	inline size_t GetPipelineIndex() const { return pipelineIndex; }
	inline Pipeline* GetPipeline() const { return pipelines.at(pipelineIndex); }

	Renderable(const Renderable& renderable) = delete;
	Renderable operator=(const Renderable& renderable) = delete;
protected:
	static std::vector<Pipeline*> pipelines;

	Window* window;

	size_t pipelineIndex;
};

#endif