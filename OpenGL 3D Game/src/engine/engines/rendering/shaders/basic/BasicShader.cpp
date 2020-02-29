#include "BasicShader.h"
#include "engine/core/gameObject/GameObject.h"
#include "engine/core/movement/Camera.h"

BasicShader::BasicShader() : Shader()
{
	AddShader(ResourceLoader::LoadShader("basic_VS.glsl"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShader("basic_FS.glsl"), FRAGMENT_SHADER);
	CompileShader();

	AddUniform("m_transform");
	AddUniform("m_view");
	AddUniform("m_projection");
}

void BasicShader::UpdateUniforms(GameObject* object, Camera* camera) const
{
	SetUniform("m_transform", object->GetWorldTransform()->getTransformationMatrix());
	SetUniform("m_view", camera->GetViewMatrix());
	SetUniform("m_projection", camera->GetProjectionMatrix());
}