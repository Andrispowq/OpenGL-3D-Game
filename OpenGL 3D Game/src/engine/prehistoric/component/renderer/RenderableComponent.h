#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H

#include "engine/prehistoric/component/Component.h"

#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/framework/Window.h"

#include "engine/prehistoric/core/util/Includes.hpp"

class AssembledAssetManager;

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
	RenderableComponent(Pipeline* pipeline, Window* window, AssembledAssetManager* manager);
	RenderableComponent(Window* window, AssembledAssetManager* manager);
	virtual ~RenderableComponent();

	static void RecreatePipelines();

	virtual void Render(Renderer* renderer) const = 0;
	virtual void BatchRender() const = 0;

	inline size_t getPipelineIndex() const { return pipelineIndex; }
	Pipeline* getPipeline() const;

	inline RenderPriority getPriority() const { return priority; }
	inline void setPriority(RenderPriority priority) { this->priority = priority; }

	RenderableComponent(const RenderableComponent&) = delete;
	RenderableComponent(const RenderableComponent&&) = delete;
	RenderableComponent& operator=(RenderableComponent) = delete;
protected:
	AssembledAssetManager* manager;

	Window* window;

	size_t pipelineIndex;
	RenderPriority priority;
};

#endif