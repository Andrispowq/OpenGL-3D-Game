#include "engine/prehistoric/core/util/Includes.hpp"
#include "RenderableComponent.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/rendering/pipeline/GraphicsPipeline.h"
#include "engine/prehistoric/common/rendering/pipeline/ComputePipeline.h"

#include "engine/platform/vulkan/rendering/shaders/VKShader.h"

#include "engine/prehistoric/resources/AssembledAssetManager.h"

RenderableComponent::RenderableComponent(Pipeline* pipeline, Window* window, AssembledAssetManager* manager)
	: priority(RenderPriority::_3D)
{
	this->window = window;
	this->manager = manager;

	pipelineIndex = manager->loadResource<Pipeline>(pipeline);

	//Just in case, but this might be added as optional because it resets the viewport and the scissor
	pipeline->setViewportStart(0);
	pipeline->setViewportSize({ (float) FrameworkConfig::windowWidth, (float) FrameworkConfig::windowHeight });
	pipeline->setScissorStart(0);
	pipeline->setScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });
}

RenderableComponent::RenderableComponent(Window* window, AssembledAssetManager* manager)
	: priority(RenderPriority::_3D)
{
	pipelineIndex = -1;

	this->window = window;
	this->manager = manager;
}

RenderableComponent::~RenderableComponent()
{
	manager->removeReference<Pipeline>(pipelineIndex);
	pipelineIndex = -1;
}

void RenderableComponent::RecreatePipelines()
{
	/*for (Pipeline* pipeline : pipelines)
	{
		pipeline->setViewportStart(0);
		pipeline->setViewportSize({ (float)FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight });
		pipeline->setScissorStart(0);
		pipeline->setScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

		pipeline->RecreatePipeline();
	}*/
}

Pipeline* RenderableComponent::getPipeline() const
{
	return manager->getResourceByID<Pipeline>(pipelineIndex);
}
