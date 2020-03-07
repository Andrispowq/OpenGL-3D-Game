#include "GLBasicShader.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/movement/Camera.h"
#include "engine/prehistoric/component/renderer/Renderer.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/prehistoric/common/model/Texture.h"
#include "engine/prehistoric/core/util/Time.h"

GLBasicShader::GLBasicShader() : GLShader()
{
	AddShader(ResourceLoader::LoadShader("basic_VS.glsl"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShader("basic_FS.glsl"), FRAGMENT_SHADER);
	CompileShader();

	AddUniform("m_transform");
	AddUniform("m_view");
	AddUniform("m_projection");

	AddUniform("diffuse");
}

void GLBasicShader::UpdateUniforms(GameObject* object, Camera* camera) const
{
	Material* material = dynamic_cast<Renderer*>(object->GetComponent("renderer"))->GetMaterial();

	SetUniform("m_transform", object->GetWorldTransform()->getTransformationMatrix());
	SetUniform("m_view", camera->GetViewMatrix());
	SetUniform("m_projection", camera->GetProjectionMatrix());

	material->GetTexture("diffuse")->Bind(0);
	SetUniformi("diffuse", 0);
}