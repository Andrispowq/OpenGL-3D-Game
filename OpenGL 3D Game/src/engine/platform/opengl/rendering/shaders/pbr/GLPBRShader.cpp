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
		std::string name = "lights[" + std::to_string(i);

		AddUniform(name + "].position");
		AddUniform(name + "].colour");
		AddUniform(name + "].intensity");
	}

	AddUniform("gamma");
	AddUniform("exposure");
	AddUniform("highDetailRange");
	AddUniform("numberOfLights");
}

void GLPBRShader::UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights, uint32_t instance_index) const
{
	SetUniform("m_view", camera->getViewMatrix());
	SetUniform("m_projection", camera->getProjectionMatrix());
	SetUniform("cameraPosition", camera->getPosition());

	uint32_t nOfLights = EngineConfig::lightsMaxNumber > (uint32_t)lights.size() ? (uint32_t)lights.size() : EngineConfig::lightsMaxNumber;
	SetUniformi("numberOfLights", nOfLights);
#if defined(PR_DEBUG)
	for (unsigned int i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::string uniformName = "lights[" + std::to_string(i) + "].";

		if (i < lights.size())
		{
			Light* light = lights[i];

			SetUniform(uniformName + "position", light->getParent()->getWorldTransform()->getPosition());
			SetUniform(uniformName + "colour", light->getColour());
			SetUniform(uniformName + "intensity", light->getIntensity());
		}
		else
		{
			//Load some dummy values for debug mode so we don't access undefined memory while debugging, but we won't in release mode
			SetUniform(uniformName + "position", Vector3f());
			SetUniform(uniformName + "colour", Vector3f());
			SetUniform(uniformName + "intensity", Vector3f());
		}
	}
#else
	for (unsigned int i = 0; i < nOfLights; i++)
	{
		std::string uniformName = "lights[" + std::to_string(i) + "].";

		Light* light = lights[i];

		SetUniform(uniformName + "position", light->getParent()->getWorldTransform()->getPosition());
		SetUniform(uniformName + "colour", light->getColour());
		SetUniform(uniformName + "intensity", light->getIntensity());
	}
#endif

	SetUniformf("gamma", EngineConfig::rendererGamma);
	SetUniformf("exposure", EngineConfig::rendererExposure);
	SetUniformi("highDetailRange", EngineConfig::rendererHighDetailRange);
}

void GLPBRShader::UpdateObjectUniforms(GameObject* object, uint32_t instance_index) const
{
	Material* material = dynamic_cast<RendererComponent*>(object->GetComponent(RENDERER_COMPONENT))->getMaterial();

	SetUniform("m_transform", object->getWorldTransform()->getTransformationMatrix());

	material->getTexture("albedoMap")->Bind(0);
	SetUniformi("material.albedoMap", 0);
	material->getTexture("normalMap")->Bind(1);
	SetUniformi("material.normalMap", 1);
	material->getTexture("displacementMap")->Bind(2);
	SetUniformi("material.displacementMap", 2);
	material->getTexture("metallicMap")->Bind(3);
	SetUniformi("material.metallicMap", 3);
	material->getTexture("roughnessMap")->Bind(4);
	SetUniformi("material.roughnessMap", 4);
	material->getTexture("occlusionMap")->Bind(5);
	SetUniformi("material.occlusionMap", 5);
	material->getTexture("emissionMap")->Bind(6);
	SetUniformi("material.emissionMap", 6);

	SetUniform("material.colour", material->getVector3f("colour"));
	SetUniformi("material.usesNormalMap", material->exists("normalMap"));
	SetUniformf("material.heightScale", material->getFloat("heightScale"));
	SetUniformf("material.metallic", material->getFloat("metallic"));
	SetUniformf("material.roughness", material->getFloat("roughness"));
	SetUniformf("material.ambientOcclusion", material->getFloat("ambientOcclusion"));
	SetUniform("material.emission", material->getVector3f("emission"));
}