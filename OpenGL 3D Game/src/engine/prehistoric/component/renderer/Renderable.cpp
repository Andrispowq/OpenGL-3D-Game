#include "engine/prehistoric/core/util/Includes.hpp"
#include "Renderable.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"

std::vector<MeshVBO*> Renderable::vbos;
std::vector<Pipeline*> Renderable::pipelines;

Renderable::Renderable(MeshVBO* vbo, Pipeline* pipeline)
{
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

Renderable::Renderable()
{
	vboIndex = -1;
	pipelineIndex = -1;
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
	for (MeshVBO* vbo : vbos)
	{
		delete vbo;
	}

	for (Pipeline* pipeline : pipelines)
	{
		pipeline->DestroyPipeline();
		delete pipeline;
	}
}