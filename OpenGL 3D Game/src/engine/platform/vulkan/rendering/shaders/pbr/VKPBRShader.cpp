#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VKPBRShader.h"

VKPBRShader::VKPBRShader(Window* window) : VKShader(window->GetContext(), window->GetSwapchain())
{
	AddShader(ResourceLoader::LoadShaderVK("vulkan/pbr/pbr_VS.spv"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShaderVK("vulkan/pbr/pbr_GS.spv"), GEOMETRY_SHADER);
	AddShader(ResourceLoader::LoadShaderVK("vulkan/pbr/pbr_FS.spv"), FRAGMENT_SHADER);

	AddUniform("camera", VERTEX_SHADER | GEOMETRY_SHADER | FRAGMENT_SHADER, UniformBuffer, 0, 0, sizeof(float) * 16 * 2 + Vector3f::size());
	AddUniform("lightConditions", GEOMETRY_SHADER | FRAGMENT_SHADER, UniformBuffer, 0, 1, sizeof(int) * 2 + sizeof(float) + Vector3f::size());
	AddUniform("lights", FRAGMENT_SHADER, UniformBuffer, 0, 2, Vector4f::size() * 3 * EngineConfig::lightsMaxNumber);

	AddUniform("m_transform", VERTEX_SHADER, UniformBuffer, 2, 0, sizeof(float) * 16);

	AddUniform("material", GEOMETRY_SHADER | FRAGMENT_SHADER, UniformBuffer, 1, 0, Vector3f::size() * 2 + sizeof(int) + sizeof(float) * 4);

	AddUniform(ALBEDO_MAP, GEOMETRY_SHADER | FRAGMENT_SHADER, CombinedImageSampler, 1, 1, 0, nullptr);
	AddUniform(DISPLACEMENT_MAP, GEOMETRY_SHADER | FRAGMENT_SHADER, CombinedImageSampler, 1, 2, 0, nullptr);
	AddUniform(NORMAL_MAP, GEOMETRY_SHADER | FRAGMENT_SHADER, CombinedImageSampler, 1, 3, 0, nullptr);
	AddUniform(METALLIC_MAP, GEOMETRY_SHADER | FRAGMENT_SHADER, CombinedImageSampler, 1, 4, 0, nullptr);
	AddUniform(ROUGHNESS_MAP, GEOMETRY_SHADER | FRAGMENT_SHADER, CombinedImageSampler, 1, 5, 0, nullptr);
	AddUniform(OCCLUSION_MAP, GEOMETRY_SHADER | FRAGMENT_SHADER, CombinedImageSampler, 1, 6, 0, nullptr);
	AddUniform(EMISSION_MAP, GEOMETRY_SHADER | FRAGMENT_SHADER, CombinedImageSampler, 1, 7, 0, nullptr);
}

void VKPBRShader::UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights, uint32_t instance_index) const
{
	//Offset values are copied from shaders
	SetUniform("camera", camera->getViewMatrix(), 0, instance_index);
	SetUniform("camera", camera->getProjectionMatrix(), 64, instance_index);
	SetUniform("camera", camera->getPosition(), 128, instance_index);

	SetUniformi("lightConditions", EngineConfig::rendererHighDetailRange, 0, instance_index);
	SetUniformi("lightConditions", (uint32_t)lights.size() >= EngineConfig::lightsMaxNumber ? EngineConfig::lightsMaxNumber : (uint32_t)lights.size(), 4, instance_index);
	SetUniformf("lightConditions", EngineConfig::rendererGamma, 8, instance_index);

	size_t baseOffset = EngineConfig::lightsMaxNumber * Vector4f::size();

	for (uint32_t i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		size_t currentOffset = Vector4f::size() * i;

		if (i < lights.size())
		{
			Light* light = lights[i];

			SetUniform("lights", Vector4f(light->GetParent()->getWorldTransform()->GetPosition(), 0), baseOffset * 0 + currentOffset, instance_index);
			SetUniform("lights", Vector4f(light->GetColour(), 0), baseOffset * 1 + currentOffset, instance_index);
			SetUniform("lights", Vector4f(light->GetIntensity(), 0), baseOffset * 2 + currentOffset, instance_index);
		}
		else
		{
			SetUniform("lights", Vector4f(), baseOffset * 0 + currentOffset, instance_index);
			SetUniform("lights", Vector4f(), baseOffset * 1 + currentOffset, instance_index);
			SetUniform("lights", Vector4f(), baseOffset * 2 + currentOffset, instance_index);
		}
	}

  	BindSet(swapchain->GetDrawCommandBuffer(), 0, instance_index);
}

void VKPBRShader::UpdateObjectUniforms(GameObject* object, uint32_t instance_index) const
{
	Material* material = ((Renderer*)object->GetComponent(RENDERER_COMPONENT))->GetMaterial();

	//Offset values are copied from shaders
	SetUniform("m_transform", object->getWorldTransform()->getTransformationMatrix(), instance_index);

	SetUniform("material", material->GetVector3f(COLOUR), 0, instance_index);
	SetUniform("material", material->GetVector3f(EMISSION), 16, instance_index);
	SetUniformi("material", material->GetTexture(NORMAL_MAP)->getID() != Material::GetDefaultTexture()->getID(), 32, instance_index);
	SetUniformf("material", material->GetFloat(HEIGHT_SCALE), 36, instance_index);
	SetUniformf("material", material->GetFloat(METALLIC), 40, instance_index);
	SetUniformf("material", material->GetFloat(ROUGHNESS), 44, instance_index);
	SetUniformf("material", material->GetFloat(OCCLUSION), 48, instance_index);

	SetTexture(ALBEDO_MAP, material->GetTexture(ALBEDO_MAP), instance_index);
	SetTexture(DISPLACEMENT_MAP, material->GetTexture(DISPLACEMENT_MAP), instance_index);
	SetTexture(NORMAL_MAP, material->GetTexture(NORMAL_MAP), instance_index);
	SetTexture(METALLIC_MAP, material->GetTexture(METALLIC_MAP), instance_index);
	SetTexture(ROUGHNESS_MAP, material->GetTexture(ROUGHNESS_MAP), instance_index);
	SetTexture(OCCLUSION_MAP, material->GetTexture(OCCLUSION_MAP), instance_index);
	SetTexture(EMISSION_MAP, material->GetTexture(EMISSION_MAP), instance_index);

	BindSet(swapchain->GetDrawCommandBuffer(), 1, instance_index);
	BindSet(swapchain->GetDrawCommandBuffer(), 2, instance_index);
}