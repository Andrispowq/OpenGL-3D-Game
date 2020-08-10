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
	AddUniform("exposure");
}

void GLBasicShader::UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights, uint32_t instance_index) const
{
	SetUniform("m_view", camera->getViewMatrix());
	SetUniform("m_projection", camera->getProjectionMatrix());
	SetUniform("cameraPosition", camera->getPosition());

	uint32_t nOfLights = EngineConfig::lightsMaxNumber > (uint32_t)lights.size() ? (uint32_t)lights.size() : EngineConfig::lightsMaxNumber;
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
}

void GLBasicShader::UpdateObjectUniforms(GameObject* object, uint32_t instance_index) const
{
	Material* material = dynamic_cast<RendererComponent*>(object->GetComponent("RendererComponent"))->getMaterial();

	SetUniform("m_transform", object->getWorldTransform()->getTransformationMatrix());

	material->getTexture("albedoMap")->Bind(0);
	SetUniformi("material.albedoMap", 0);
	material->getTexture("metallicMap")->Bind(1);
	SetUniformi("material.metallicMap", 2);
	material->getTexture("roughnessMap")->Bind(2);
	SetUniformi("material.roughnessMap", 2);

	SetUniform("material.colour", material->getVector3f("colour"));
	SetUniformf("material.metallic", material->getFloat("metallic"));
	SetUniformf("material.roughness", material->getFloat("roughness"));
}