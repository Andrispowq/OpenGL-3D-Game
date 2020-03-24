#include "engine/prehistoric/core/util/Includes.hpp"
#include "Renderer.h"
#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"
#include "engine/prehistoric/common/rendering/RenderingEngine.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/model/material/Material.h"

std::vector<Material*> Renderer::materials;

Renderer::Renderer(MeshVBO* vbo, Pipeline* pipeline, Material* material) : Renderable(vbo, pipeline)
{
	auto mInd = std::find(materials.begin(), materials.end(), material);

	if (mInd == materials.end())
	{
		materials.push_back(material);
		this->materialIndex = materials.size() - 1;
	}
	else
	{
		this->materialIndex = std::distance(materials.begin(), mInd);
		//delete material;
	}
}

Renderer::Renderer() : Renderable()
{
	materials.push_back(new Material());
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
	MeshVBO* vbo = vbos[vboIndex];
	Pipeline* pipeline = pipelines[pipelineIndex];
	
	pipeline->BindPipeline();
	pipeline->GetShader()->UpdateUniforms(parent, renderingEngine.GetCamera(), renderingEngine.GetLights());

	vbo->Bind();
	vbo->Draw();
	vbo->Unbind();

	pipeline->UnbindPipeline();
}

void Renderer::BatchRender(const RenderingEngine& renderingEngine) const
{
	MeshVBO* vbo = vbos[vboIndex];
	Pipeline* pipeline = pipelines[pipelineIndex];

	pipeline->GetShader()->UpdateUniforms(parent, renderingEngine.GetCamera(), renderingEngine.GetLights());

	vbo->Draw();
}