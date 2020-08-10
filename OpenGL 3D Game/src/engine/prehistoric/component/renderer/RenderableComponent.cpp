#include "engine/prehistoric/core/util/Includes.hpp"
#include "RenderableComponent.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/rendering/pipeline/GraphicsPipeline.h"
#include "engine/prehistoric/common/rendering/pipeline/ComputePipeline.h"

#include "engine/platform/vulkan/rendering/shaders/VkShader.h"

std::vector<Pipeline*> RenderableComponent::pipelines;

RenderableComponent::RenderableComponent(Pipeline* pipeline, Window* window)
	: priority(RenderPriority::_3D)
{
	this->window = window;

	size_t index;
	if ((index = FindElement(pipeline, pipelines)) == 0xFFFFFFFF)
	{
		pipelines.push_back(pipeline);
		this->pipelineIndex = pipelines.size() - 1;
	}
	else
	{
		this->pipelineIndex = index;
	}

	//Just in case, but this might be added as optional because it resets the viewport and the scissor
	pipeline->setViewportStart(0);
	pipeline->setViewportSize({ (float) FrameworkConfig::windowWidth, (float) FrameworkConfig::windowHeight });
	pipeline->setScissorStart(0);
	pipeline->setScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });
}

RenderableComponent::RenderableComponent(Window* window)
	: priority(RenderPriority::_3D)
{
	pipelineIndex = -1;

	this->window = window;
}

RenderableComponent::~RenderableComponent()
{
}

void RenderableComponent::RecreatePipelines()
{
	for (Pipeline* pipeline : pipelines)
	{
		pipeline->setViewportStart(0);
		pipeline->setViewportSize({ (float)FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight });
		pipeline->setScissorStart(0);
		pipeline->setScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

		pipeline->RecreatePipeline();
	}
}

void RenderableComponent::CleanUp()
{
	for (Pipeline* pipeline : pipelines)
	{
		delete pipeline;
	}
}