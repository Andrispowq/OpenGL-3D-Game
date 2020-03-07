#include "Renderer.h"
#include "engine/prehistoric/common/rendering/RenderingEngine.h"
#include "engine/prehistoric/common/rendering/shaders/Shader.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/model/material/Material.h"

Renderer::Renderer(MeshVBO* vbo, Shader* shader, Material* material) : Renderable(vbo, shader)
{
	this->material = material;
}

Renderer::Renderer() : Renderable()
{
	this->material = new Material();
}

Renderer::~Renderer()
{
	delete material;
}

void Renderer::PreRender(RenderingEngine* renderingEngine)
{
	renderingEngine->AddModel(this);
}

void Renderer::Render(const RenderingEngine& renderingEngine) const
{
	MeshVBO* vbo = vbos.at(vboIndex);
	Shader* shader = shaders.at(shaderIndex);

	shader->Bind();
	shader->UpdateUniforms(parent, renderingEngine.GetCamera());

	vbo->Bind();
	vbo->Draw();
	vbo->Unbind();

	shader->Unbind();
}

void Renderer::BatchRender(const RenderingEngine& renderingEngine) const
{
	MeshVBO* vbo = vbos.at(vboIndex);
	Shader* shader = shaders.at(shaderIndex);

	vbo->Draw();
}