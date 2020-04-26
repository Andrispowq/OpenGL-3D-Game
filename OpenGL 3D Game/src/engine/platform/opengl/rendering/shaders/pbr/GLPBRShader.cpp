#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLPBRShader.h"

GLPBRShader::GLPBRShader() : GLShader()
{
	AddShader(ResourceLoader::LoadShaderGL("opengl/pbr/pbr_VS.glsl"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/pbr/pbr_GS.glsl"), GEOMETRY_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/pbr/pbr_FS.glsl"), FRAGMENT_SHADER);
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

	for (uint32_t i = 0; i < EngineConfig::lightsMaxNumber; i++)
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
	AddUniform("numberOfLights");
}

void GLPBRShader::UpdateShaderUniforms(Camera* camera, std::vector<Light*> lights) const
{
	SetUniform("m_view", camera->getViewMatrix());
	SetUniform("m_projection", camera->getProjectionMatrix());
	SetUniform("cameraPosition", camera->getPosition());

	SetUniformi("numberOfLights", (uint32_t)lights.size() > EngineConfig::lightsMaxNumber ? EngineConfig::lightsMaxNumber : (uint32_t)lights.size());
	for (uint32_t i = 0; i < EngineConfig::lightsMaxNumber; i++)
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

	SetUniformf("gamma", EngineConfig::rendererGamma);
	SetUniformi("highDetailRange", EngineConfig::rendererHighDetailRange);
}

void GLPBRShader::UpdateObjectUniforms(GameObject* object) const
{
	Material* material = dynamic_cast<Renderer*>(object->GetComponent("Renderer"))->GetMaterial();

	SetUniform("m_transform", object->GetWorldTransform()->getTransformationMatrix());

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
}