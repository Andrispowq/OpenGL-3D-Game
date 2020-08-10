#include "engine/prehistoric/core/util/Includes.hpp"
#include "RendererComponent.h"
#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/engines/RenderingEngine.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/model/material/Material.h"

std::vector<Material*> RendererComponent::materials;

RendererComponent::RendererComponent(Pipeline* pipeline, Material* material, Window* window)
	: RenderableComponent(pipeline, window)
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

RendererComponent::RendererComponent(Window* window)
	: RenderableComponent(window)
{
	size_t index;
	if ((index = FindElement((Material*)nullptr, materials)) == 0xFFFFFFFF)
	{
		materials.push_back(nullptr);
		this->materialIndex = materials.size() - 1;
	}
	else
	{
		this->materialIndex = index;
	}
}

RendererComponent::~RendererComponent()
{
	materialIndex = -1;
}

void RendererComponent::CleanUp()
{
	for (Material* material : materials)
	{
		if(material != nullptr)
			delete material;
	}

	RenderableComponent::CleanUp();
}

void RendererComponent::PreRender(Renderer* renderer)
{
	renderer->AddModel(this);
}

void RendererComponent::Render(Renderer* renderer) const
{
	Pipeline* pipeline = pipelines[pipelineIndex];
	
	pipeline->BindPipeline();
	pipeline->getShader()->UpdateShaderUniforms(renderer->getCamera(), renderer->getLights());
	pipeline->getShader()->UpdateSharedUniforms(parent);
	pipeline->getShader()->UpdateObjectUniforms(parent);

	pipeline->RenderPipeline();
	pipeline->UnbindPipeline();
}

void RendererComponent::BatchRender() const
{
	Pipeline* pipeline = pipelines[pipelineIndex];

	pipeline->getShader()->UpdateObjectUniforms(parent, shader_instance_index);
	pipeline->RenderPipeline();
}