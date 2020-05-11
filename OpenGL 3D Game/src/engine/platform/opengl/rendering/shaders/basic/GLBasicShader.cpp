#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLBasicShader.h"

GLBasicShader::GLBasicShader() : GLShader()
{
	AddShader(ResourceLoader::LoadShaderGL("opengl/basic/basic_VS.glsl"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/basic/basic_FS.glsl"), FRAGMENT_SHADER);
	CompileShader();

	AddUniform("m_transform");
	AddUniform("m_view");
	AddUniform("m_projection");

	AddUniform("cameraPosition");

	for (uint32_t i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::string name = "lights[" + std::to_string(i);

		AddUniform(name + "].position");
		AddUniform(name + "].colour");
		AddUniform(name + "].intensity");
	}

	AddUniform("material.albedoMap");
	AddUniform("material.metallicMap");
	AddUniform("material.roughnessMap");

	AddUniform("material.colour");
	AddUniform("material.metallic");
	AddUniform("material.roughness");

	AddUniform("gamma");
}

void GLBasicShader::UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights) const
{
	SetUniform("m_view", camera->getViewMatrix());
	SetUniform("m_projection", camera->getProjectionMatrix());
	SetUniform("cameraPosition", camera->getPosition());

	for (uint32_t i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::string name = "lights[" + std::to_string(i);

		if (i < lights.size())
		{
			SetUniform(name + "].position", lights[i]->GetParent()->GetWorldTransform()->GetPosition());
			SetUniform(name + "].colour", lights[i]->GetColour());
			SetUniform(name + "].intensity", lights[i]->GetIntensity());
		}
		else
		{
			SetUniform(name + "].position", Vector3f());
			SetUniform(name + "].colour", Vector3f());
			SetUniform(name + "].intensity", Vector3f());
		}
	}

	SetUniformf("gamma", EngineConfig::rendererGamma);
}

void GLBasicShader::UpdateObjectUniforms(GameObject* object) const
{
	Material* material = dynamic_cast<Renderer*>(object->GetComponent("Renderer"))->GetMaterial();

	SetUniform("m_transform", object->GetWorldTransform()->getTransformationMatrix());

	material->GetTexture("albedoMap")->Bind(0);
	SetUniformi("material.albedoMap", 0);
	material->GetTexture("metallicMap")->Bind(1);
	SetUniformi("material.metallicMap", 2);
	material->GetTexture("roughnessMap")->Bind(2);
	SetUniformi("material.roughnessMap", 2);

	SetUniform("material.colour", material->GetVector3f("colour"));
	SetUniformf("material.metallic", material->GetFloat("metallic"));
	SetUniformf("material.roughness", material->GetFloat("roughness"));
}