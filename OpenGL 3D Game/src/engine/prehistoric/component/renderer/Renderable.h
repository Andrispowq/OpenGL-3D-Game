#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "engine/prehistoric/component/Component.h"

#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/framework/Window.h"
#include "engine/prehistoric/engines/RenderingEngine.h"

#include "engine/prehistoric/core/util/Includes.hpp"

enum class RenderPriority
{
	_3D = 0,
	_TRANSPARENCY = 1,
	_2D = 2,
	_POST_PROCESSING = 3
};

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

	static std::vector<Pipeline*> getPipelines() { return pipelines; }

	inline size_t getPipelineIndex() const { return pipelineIndex; }
	inline Pipeline* getPipeline() const { return pipelines.at(pipelineIndex); }

	inline RenderPriority getPriority() const { return priority; }
	inline void setPriority(RenderPriority priority) { this->priority = priority; }

	Renderable(const Renderable& renderable) = delete;
	Renderable operator=(const Renderable& renderable) = delete;
	Renderable(const Renderable&& renderable) = delete;
	Renderable operator=(const Renderable&& renderable) = delete;
protected:
	static std::vector<Pipeline*> pipelines;

	Window* window;

	size_t pipelineIndex;
	uint32_t shader_instance_index;

	RenderPriority priority;
};

#endif