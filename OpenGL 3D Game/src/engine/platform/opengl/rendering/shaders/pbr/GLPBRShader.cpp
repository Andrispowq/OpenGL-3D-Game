#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLPBRShader.h"
#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/movement/Camera.h"
#include "engine/prehistoric/component/renderer/Renderer.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/platform/opengl/texture/GLTexture.h"
#include "engine/config/EngineConfig.h"

GLPBRShader::GLPBRShader() : GLShader()
{
	AddShader(ResourceLoader::LoadShader("pbr/pbr_VS.glsl"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShader("pbr/pbr_GS.glsl"), GEOMETRY_SHADER);
	AddShader(ResourceLoader::LoadShader("pbr/pbr_FS.glsl"), FRAGMENT_SHADER);
	CompileShader();

	AddUniform("m_transform");
	AddUniform("m_view");
	AddUniform("m_projection");

	AddUniform("cameraPosition");
	AddUniform("highDetailRange");

	AddUniform("material.albedoMap");
	AddUniform("material.normalMap");
	AddUniform("material.displacementMap");
	AddUniform("material.roughnessMap");
	AddUniform("material.metallicMap");
	AddUniform("material.occlusionMap");
	AddUniform("material.emissionMap");

	AddUniform("material.colour");
	AddUniform("material.usesNormalMap");
	AddUniform("material.heightScale");
	AddUniform("material.roughness");
	AddUniform("material.metallic");
	AddUniform("material.ambientOcclusion");
	AddUniform("material.emission");

	for (unsigned int i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::string name;
		name += "lights[";
		name += std::to_string(i);

		AddUniform(name + "].position");
		AddUniform(name + "].colour");
		AddUniform(name + "].intensity");
	}

	AddUniform("gamma");
	AddUniform("highDetailRange");
}

void GLPBRShader::UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const
{
	Material* material = dynamic_cast<Renderer*>(object->GetComponent("Renderer"))->GetMaterial();

	SetUniform("m_transform", object->GetWorldTransform()->getTransformationMatrix());
	SetUniform("m_view", camera->GetViewMatrix());
	SetUniform("m_projection", camera->GetProjectionMatrix());

	SetUniform("cameraPosition", camera->GetPosition());

	for (unsigned int i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::string name;
		name += "lights[";
		name += std::to_string(i);

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

	material->GetTexture("albedoMap")->Bind(0);
	SetUniformi("material.albedoMap", 0);
	material->GetTexture("normalMap")->Bind(1);
	SetUniformi("material.normalMap", 1);
	material->GetTexture("displacementMap")->Bind(2);
	SetUniformi("material.displacementMap", 2);
	material->GetTexture("metallicMap")->Bind(3);
	SetUniformi("material.metallicMap", 3);
	material->GetTexture("roughnessMap")->Bind(4);
	SetUniformi("material.roughnessMap", 4);
	material->GetTexture("occlusionMap")->Bind(5);
	SetUniformi("material.occlusionMap", 5);
	material->GetTexture("emissionMap")->Bind(6);
	SetUniformi("material.emissionMap", 6);

	SetUniform("material.colour", material->GetVector3f("colour"));
	SetUniformi("material.usesNormalMap", dynamic_cast<GLTexture*>(material->GetTexture("normalMap"))->getID() != 
										dynamic_cast<GLTexture*>(Material::GetDefaultTexture())->getID());
	SetUniformf("material.heightScale", material->GetFloat("heightScale"));
	SetUniformf("material.metallic", material->GetFloat("metallic"));
	SetUniformf("material.roughness", material->GetFloat("roughness"));
	SetUniformf("material.ambientOcclusion", material->GetFloat("ambientOcclusion"));
	SetUniform("material.emission", material->GetVector3f("emission"));

	SetUniformf("gamma", EngineConfig::rendererGamma);
	SetUniformi("highDetailRange", EngineConfig::rendererHighDetailRange);
}