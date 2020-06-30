#ifndef VK_SHADER_H
#define VK_SHADER_H

#include "engine/prehistoric/common/rendering/shaders/Shader.h"

#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"
#include "engine/platform/vulkan/framework/device/VKDevice.h"

#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/util/Includes.hpp"

struct UniformBufferObject
{
	Matrix4f transformation;
	Matrix4f view;
	Matrix4f projection;
};

class VKShader : public Shader
{
public:
	VKShader(Context* context, Swapchain* swapchain, const std::vector<char>* files, uint32_t length);
	VKShader(Context* context, Swapchain* swapchain);

	virtual ~VKShader();

	virtual void Bind(void* commandBuffer) const override {}
	virtual void Unbind() const override {}

	void BindSet(void* commandBuffer, uint32_t set, uint32_t instance_index = 0) const;

	virtual bool AddUniform(const std::string& name, uint32_t stages, UniformType type, uint32_t set, uint32_t binding, size_t size, Texture* texture = nullptr) override;
	virtual bool AddUniformBlock(const std::string& name, uint32_t stages, UniformType type, uint32_t set, uint32_t binding, size_t size, Texture* texture = nullptr) override { return true; }

	virtual bool AddShader(const std::vector<char>& code, ShaderType type) override;
	virtual bool CompileShader() const override { return true; }

	//Uniform handling
	virtual void SetUniformi(const std::string& name, int value, size_t offset = 0, uint32_t instance_index = 0) const override;
	virtual void SetUniformf(const std::string& name, float value, size_t offset = 0, uint32_t instance_index = 0) const override;

	virtual void SetUniform(const std::string& name, const Vector2f& value, size_t offset = 0, uint32_t instance_index = 0) const override;
	virtual void SetUniform(const std::string& name, const Vector3f& value, size_t offset = 0, uint32_t instance_index = 0) const override;
	virtual void SetUniform(const std::string& name, const Vector4f& value, size_t offset = 0, uint32_t instance_index = 0) const override;
	virtual void SetUniform(const std::string& name, const Matrix4f& value, size_t offset = 0, uint32_t instance_index = 0) const override;

	virtual void SetTexture(const std::string& name, Texture* value, uint32_t instance_index = 0) const override;

	virtual void SetUniform(const std::string& name, const void* value, size_t size, size_t offset = 0, uint32_t instance_index = 0) const override;

	virtual void BindUniformBlock(const std::string& name, uint32_t binding, uint32_t instance_index = 0) const override {}

	virtual void UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights, uint32_t instance_index = 0) const override = 0;
	virtual void UpdateObjectUniforms(GameObject* object, uint32_t instance_index = 0) const override = 0;

	virtual bool operator==(const Shader& other)
	{
		if (counter != (*reinterpret_cast<const VKShader*>(&other)).counter)
			return false;

		for (uint32_t i = 0; i < counter; i++)
		{
			auto& stage = shaderStages[i];
			auto& ostage = (*reinterpret_cast<const VKShader*>(&other)).shaderStages[i];

			//TODO: This does not work, and I know why, but it's not used so I'll leave it in here for now
			if (stage.flags != ostage.flags || stage.module != ostage.module || stage.pName != ostage.pName || stage.pNext != ostage.pNext
				|| stage.pSpecializationInfo != ostage.pSpecializationInfo || stage.stage != ostage.stage)
				return false;
		}

		return true;
	}

	VkPipelineShaderStageCreateInfo* GetShaderStages() { return shaderStages; }
	VkPipelineLayout& GetPipelineLayout() { return pipelineLayout; }

	uint32_t getModuleCount() { return counter; }
	uint32_t getInstanceIndex() { return instance_counter; }

	void RegisterInstance();
private:
	VkShaderModule CreateShaderModule(const std::vector<char>& code) const;
	void CreateStageInfo(VkPipelineShaderStageCreateInfo& info, VkShaderModule module, const char* main, VkShaderStageFlagBits type);

	void CreateUniformBuffer(VkDeviceSize bufferSize, const std::string& name);

protected:
	VkDescriptorType GetDescriptorType(UniformType type) const;
	uint32_t GetShaderStages(uint32_t stages) const;

	//Module data
	VkShaderModule* modules;
	VkPipelineShaderStageCreateInfo* shaderStages;

	//Pipeline data
	VkPipelineLayout pipelineLayout;
	VkDescriptorPool descriptorPool;

	//Per uniform, per framebuffer data
	uint32_t numBuffers;
	uint32_t numDescriptors = 0;
	uint32_t numSets = 0;

	//Uniforms (descriptorSets and descriptorSetLayouts are wrapped in a vector because it's per shader instances)
	std::unordered_map<UniformType, VkDescriptorPoolSize> descriptorSizes; //Descriptor pool creation helper
	std::unordered_map<uint32_t, std::vector<std::vector<VkDescriptorSet>>> descriptorSets; //We can access the descriptor sets easily with a number
	std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayout>> descriptorSetLayouts; //We can access the descriptor set layout easily with a number
	std::unordered_map<uint32_t, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>> bindings; //We can access the set layout binding for each binding with the set and the binding

	std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> setBindings; //This stores the set and binding per uniform by it's name

	//The actual uniform data (they are wrapped in a vector because we need to have a separate uniform buffer and memory for every single instance of this shader
	//std::string : uniform name, pair: the uniforms and the size of ONE uniforms (buffer or memory), outer vector: per swapchain image, inner vector: per instance of the shader
	std::unordered_map<std::string, std::pair<std::vector<std::vector<VkBuffer>>, size_t>> uniformBuffers;
	std::unordered_map<std::string, std::pair<std::vector<std::vector<VkDeviceMemory>>, size_t>> uniformBuffersMemories;

	//std::string : uniform name, outer vector: per swapchain image, inner vector: per instance of the shader
	std::unordered_map<std::string, std::vector<std::vector<VkImageView>>> imageViews;
	std::unordered_map<std::string, std::vector<std::vector<VkSampler>>> samplers;

	//This stores the uniforms' types so we can access the right uniform type
	std::unordered_map<std::string, UniformType> uniformTypes;

	//Outside data
	VKDevice* device;
	VKPhysicalDevice* physicalDevice;
	VKSwapchain* swapchain;

	uint32_t instance_counter = 0;
};

#endif