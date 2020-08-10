#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H

#include "engine/prehistoric/component/Component.h"

#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/framework/Window.h"

#include "engine/prehistoric/core/util/Includes.hpp"

enum class RenderPriority
{
	_3D = 0,
	_TRANSPARENCY = 1,
	_2D = 2,
	_POST_PROCESSING = 3
};

class RenderableComponent : public Component
{
public:
	RenderableComponent(Pipeline* pipeline, Window* window);
	RenderableComponent(Window* window);
	virtual ~RenderableComponent();

	static void CleanUp();
	static void RecreatePipelines();
	static std::vector<Pipeline*> getPipelines() { return pipelines; }

	virtual void Render(Renderer* renderer) const = 0;
	virtual void BatchRender() const = 0;

	inline size_t getPipelineIndex() const { return pipelineIndex; }
	inline Pipeline* getPipeline() const { return pipelines.at(pipelineIndex); }

	inline RenderPriority getPriority() const { return priority; }
	inline void setPriority(RenderPriority priority) { this->priority = priority; }

	RenderableComponent(const RenderableComponent&) = delete;
	RenderableComponent(const RenderableComponent&&) = delete;
	RenderableComponent& operator=(RenderableComponent) = delete;
protected:
	static std::vector<Pipeline*> pipelines;

	Window* window;

	size_t pipelineIndex;
	RenderPriority priority;
};

#endif