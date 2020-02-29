#include "Renderer.h"
#include "engine/engines/rendering/RenderingEngine.h"
#include "engine/engines/rendering/shaders/Shader.h"
#include "engine/core/gameObject/GameObject.h"

Renderer::Renderer(VBO* vbo, Shader* shader) : Renderable(vbo, shader)
{
}

Renderer::Renderer() : Renderable()
{
}

Renderer::~Renderer()
{
}

void Renderer::PreRender(RenderingEngine* renderingEngine)
{
	renderingEngine->AddModel(this);
}

void Renderer::Render(const RenderingEngine& renderingEngine)
{
	VBO* vbo = vbos.at(vboIndex);
	Shader* shader = shaders.at(shaderIndex);

	parent->Rotate(Vector3f(0, 0, 1));

	shader->Bind();
	shader->UpdateUniforms(parent, renderingEngine.GetCamera());
	vbo->draw();
	shader->Unbind();
}