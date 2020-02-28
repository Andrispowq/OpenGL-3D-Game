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

	shader->Bind();

	//TODO: temporary uniform management!!!!
	Transform* transform = parent->GetWorldTransform();
	shader->SetUniform("m_transform", transform->getTransformationMatrix());
	//shader->SetUniform("m_view", renderingEngine.GetCamera()->GetViewMatrix());
	shader->SetUniform("m_view", Matrix4f::Translation(renderingEngine.GetCamera()->GetPosition() * -1) * Matrix4f::View(renderingEngine.GetCamera()->GetForward(), renderingEngine.GetCamera()->GetUp()));
	shader->SetUniform("m_projection", renderingEngine.GetCamera()->GetProjectionMatrix());

	parent->Rotate(Vector3f(0, 0, 1));

	vbo->draw();
	shader->Unbind();
}