#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VKBasicShader.h"

VKBasicShader::VKBasicShader(Window* window) : VKShader(window->GetContext(), window->GetSwapchain())
{
	AddShader(ResourceLoader::LoadShaderVK("vulkan/basic/basic_VS.spv"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShaderVK("vulkan/basic/basic_FS.spv"), FRAGMENT_SHADER);
	
	AddUniform("camera", VERTEX_SHADER | FRAGMENT_SHADER, UniformBuffer, 0, 0, 2 * sizeof(float) * 16 + Vector3f::size());
	AddUniform("model", VERTEX_SHADER, UniformBuffer, 0, 1, sizeof(float) * 16);
	AddUniform("lights", FRAGMENT_SHADER, UniformBuffer, 0, 2, EngineConfig::lightsMaxNumber * 3 * Vector4f::size());
	AddUniform("material", FRAGMENT_SHADER, UniformBuffer, 0, 3, Vector3f::size() + 4 * sizeof(float));

	AddUniform("albedoMap", FRAGMENT_SHADER, CombinedImageSampler, 0, 4, 0);
	AddUniform("metallicMap", FRAGMENT_SHADER, CombinedImageSampler, 0, 5, 0);
	AddUniform("roughnessMap", FRAGMENT_SHADER, CombinedImageSampler, 0, 6, 0);

	AddUniform("gamma", FRAGMENT_SHADER, UniformBuffer, 0, 7, sizeof(float));
}

void VKBasicShader::UpdateShaderUniforms(Camera* camera, std::vector<Light*> lights) const
{
	SetUniform("camera", camera->getViewMatrix(), 16 * sizeof(float) * 0);
	SetUniform("camera", camera->getProjectionMatrix(), 16 * sizeof(float) * 1);
	SetUniform("camera", camera->getPosition(), 16 * sizeof(float) * 2);

	SetUniformf("gamma", EngineConfig::rendererGamma);

	size_t baseOffset = EngineConfig::lightsMaxNumber * Vector4f::size();

	for (uint32_t i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		size_t currentOffset = Vector4f::size() * i;

		if (i < lights.size())
		{
			Light* light = lights[i];

			SetUniform("lights", Vector4f(light->GetParent()->GetWorldTransform()->GetPosition(), 0), baseOffset * 0 + currentOffset);
			SetUniform("lights", Vector4f(light->GetColour(), 0), baseOffset * 1 + currentOffset);
			SetUniform("lights", Vector4f(light->GetIntensity(), 0), baseOffset * 2 + currentOffset);
		}
		else
		{
			SetUniform("lights", Vector4f(), baseOffset * 0 + currentOffset);
			SetUniform("lights", Vector4f(), baseOffset * 1 + currentOffset);
			SetUniform("lights", Vector4f(), baseOffset * 2 + currentOffset);
		}
	}
}

void VKBasicShader::UpdateObjectUniforms(GameObject* object) const
{
	SetUniform("model", object->GetWorldTransform()->getTransformationMatrix());

	Material* material = ((Renderer*)object->GetComponent(RENDERER_COMPONENT))->GetMaterial();

	SetTexture(ALBEDO_MAP, material->GetTexture(ALBEDO_MAP));
	SetTexture(METALLIC_MAP, material->GetTexture(METALLIC_MAP));
	SetTexture(ROUGHNESS_MAP, material->GetTexture(ROUGHNESS_MAP));
	
	SetUniform("material", material->GetVector3f(COLOUR));
	SetUniformf("material", material->GetFloat(METALLIC), Vector3f::size() + sizeof(float));
	SetUniformf("material", material->GetFloat(ROUGHNESS), Vector3f::size() + 2 * sizeof(float));
	SetUniformf("material", EngineConfig::rendererGamma, Vector3f::size() + 3 * sizeof(float));
}
