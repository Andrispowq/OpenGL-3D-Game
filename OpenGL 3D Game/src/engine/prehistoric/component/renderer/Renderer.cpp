#include "engine/prehistoric/core/util/Includes.hpp"
#include "Renderer.h"
#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/rendering/RenderingEngine.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/model/material/Material.h"

std::vector<Material*> Renderer::materials;

Renderer::Renderer(Pipeline* pipeline, Material* material, Window* window)
	: Renderable(pipeline, window)
{
	size_t index;
	if ((index = FindElement(material, materials)) == 0xFFFFFFFF)
	{
		materials.push_back(material);
		this->materialIndex = materials.size() - 1;		
	}
	else
	{
		this->materialIndex = index;
	}
}

Renderer::Renderer(Window* window)
	: Renderable(window)
{
	materials.push_back(new Material(window));
	this->materialIndex = materials.size() - 1;
}

Renderer::~Renderer()
{
	materialIndex = -1;
}

void Renderer::CleanUp()
{
	for (Material* material : materials)
	{
		delete material;
	}

	Material::CleanUp();
}

void Renderer::PreRender(RenderingEngine* renderingEngine)
{
	renderingEngine->AddModel(this);
}

void Renderer::Render(const RenderingEngine& renderingEngine) const
{
	Pipeline* pipeline = pipelines[pipelineIndex];

	pipeline->BindPipeline();
	pipeline->getShader()->UpdateUniforms(parent, renderingEngine.GetCamera(), renderingEngine.GetLights());
	pipeline->RenderPipeline();
	pipeline->UnbindPipeline();
}

void Renderer::BatchRender(const RenderingEngine& renderingEngine) const
{
	Pipeline* pipeline = pipelines[pipelineIndex];

	pipeline->getShader()->UpdateUniforms(parent, renderingEngine.GetCamera(), renderingEngine.GetLights());
	pipeline->RenderPipeline();
}