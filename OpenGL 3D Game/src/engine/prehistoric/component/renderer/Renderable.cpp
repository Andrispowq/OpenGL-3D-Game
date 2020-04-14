#include "engine/prehistoric/core/util/Includes.hpp"
#include "Renderable.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"

std::vector<VBO*> Renderable::vbos;
std::vector<Pipeline*> Renderable::pipelines;

Renderable::Renderable(VBO* vbo, Pipeline* pipeline, Window* window)
{
	this->window = window;

	auto vInd = std::find(vbos.begin(), vbos.end(), vbo);

	if (vInd == vbos.end())
	{
		vbos.push_back(vbo);
		this->vboIndex = vbos.size() - 1;
	}
	else
	{
		this->vboIndex = std::distance(vbos.begin(), vInd);
		//delete vbo;
	}

	auto pInd = std::find(pipelines.begin(), pipelines.end(), pipeline);

	if (pInd == pipelines.end())
	{
		pipelines.push_back(pipeline);
		this->pipelineIndex = pipelines.size() - 1;
	}
	else
	{
		this->pipelineIndex = std::distance(pipelines.begin(), pInd);
		//delete pipeline;
	}

	pipeline->SetViewportStart(0);
	pipeline->SetViewportSize({ (float) FrameworkConfig::windowWidth, (float) FrameworkConfig::windowHeight });
	pipeline->SetScissorStart(0);
	pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });
}

Renderable::Renderable(Window* window)
{
	vboIndex = -1;
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
		pipeline->RecreatePipeline();
	}
}

void Renderable::CleanUp()
{
	for (VBO* vbo : vbos)
	{
		delete vbo;
	}

	for (Pipeline* pipeline : pipelines)
	{
		pipeline->DestroyPipeline();
		delete pipeline;
	}
}