#include "engine/prehistoric/core/util/Includes.hpp"
#include "Renderable.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/rendering/pipeline/GraphicsPipeline.h"
#include "engine/prehistoric/common/rendering/pipeline/ComputePipeline.h"

#include "engine/platform/vulkan/rendering/shaders/VkShader.h"

std::vector<Pipeline*> Renderable::pipelines;

Renderable::Renderable(Pipeline* pipeline, Window* window)
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

	shader_instance_index = 0;

	//This is ugly code but there is no good workaround without some overhead, maybe if I find a way this will be removed
	//Please also note that VKShader::RegisterInstance() considers the Shader pipeline constant, 
	//which implies that you cannot add further uniforms, as they'd only affect the lastest instance
	if (FrameworkConfig::api == Vulkan)
	{
		shader_instance_index = reinterpret_cast<VKShader*>(pipeline->getShader())->getInstanceIndex();
		reinterpret_cast<VKShader*>(pipeline->getShader())->RegisterInstance();
	}

	//Just in case, but this might be added as optional because it resets the viewport and the scissor
	pipeline->SetViewportStart(0);
	pipeline->SetViewportSize({ (float) FrameworkConfig::windowWidth, (float) FrameworkConfig::windowHeight });
	pipeline->SetScissorStart(0);
	pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });
}

Renderable::Renderable(Window* window)
{
	pipelineIndex = -1;

	this->window = window;
}

Renderable::~Renderable()
{
}

void Renderable::RecreatePipelines()
{
	for (Pipeline* pipeline : pipelines)
	{
		//TODO: Viewport and scissor configuration respecting the previous size
		pipeline->SetViewportStart(0);
		pipeline->SetViewportSize({ (float)FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight });
		pipeline->SetScissorStart(0);
		pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

		pipeline->RecreatePipeline();
	}
}

void Renderable::CleanUp()
{
	for (Pipeline* pipeline : pipelines)
	{
		pipeline->DestroyPipeline();
		delete pipeline;
	}

	Pipeline::CleanUp();
	GraphicsPipeline::CleanUp();
	
}