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

class VKPipeline;

class VKShader : public Shader
{
public:
	VKShader(Context* context, Swapchain* swapchain, const std::vector<char>* files, uint32_t length);
	VKShader(Context* context, Swapchain* swapchain);

	virtual ~VKShader();

	virtual void Bind(void* commandBuffer) const override;
	virtual void Unbind() const override {}

	virtual bool AddUniform(const std::string& name, ShaderType stages, UniformType type, uint32_t binding, uint32_t set, size_t size, Texture* texture = nullptr) override;
	virtual bool AddUniformBlock(const std::string& name, ShaderType stages, UniformType type, uint32_t binding, uint32_t set, size_t size, Texture* texture = nullptr) override { return true; }

	virtual bool AddShader(const std::vector<char>& code, ShaderType type) override;
	virtual bool CompileShader() const override { return true; }

	//Uniform handling
	virtual void SetUniformi(const std::string& name, int value, size_t offset = 0) const override;
	virtual void SetUniformf(const std::string& name, float value, size_t offset = 0) const override;

	virtual void SetUniform(const std::string& name, const Vector2f& value, size_t offset = 0) const override;
	virtual void SetUniform(const std::string& name, const Vector3f& value, size_t offset = 0) const override;
	virtual void SetUniform(const std::string& name, const Vector4f& value, size_t offset = 0) const override;
	virtual void SetUniform(const std::string& name, const Matrix4f& value, size_t offset = 0) const override;

	virtual void SetUniform(const std::string& name, const void* value, size_t size, size_t offset = 0) const override;

	virtual void BindUniformBlock(const std::string& name, uint32_t binding) const override {}

	virtual void UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const;

	VkPipelineShaderStageCreateInfo* GetShaderStages() { return shaderStages; }
	VkPipelineLayout& GetPipelineLayout() { return pipelineLayout; }

	void RegisterPipeline(VKPipeline* pipeline) { this->pipeline = pipeline; }
private:
	bool AddShader(const std::vector<char>& code, VkShaderStageFlagBits type);
	VkShaderModule CreateShaderModule(const std::vector<char>& code) const;
	void CreateStageInfo(VkPipelineShaderStageCreateInfo& info, VkShaderModule module, const char* main, VkShaderStageFlagBits type);

	VkDescriptorType GetDescriptorType(UniformType type) const;
	uint32_t GetShaderStages(ShaderType stages) const;

	//Module data
	VkShaderModule* modules;
	VkPipelineShaderStageCreateInfo* shaderStages;

	//Pipeline data
	VkPipelineLayout pipelineLayout;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;

	//Per uniform, per framebuffer data
	uint32_t numBuffers;
	uint32_t numDescriptors = 0;
	uint32_t numSets = 0;

	std::unordered_map<UniformType, VkDescriptorPoolSize> descriptorSizes;
	std::unordered_map<uint32_t, std::vector<VkDescriptorSet>> descriptorSets;
	std::unordered_map<uint32_t, VkDescriptorSetLayout> descriptorSetLayouts;
	std::unordered_map<std::string, std::pair<uint32_t, VkDescriptorSetLayoutBinding>> bindings;
	std::unordered_map<std::string, std::pair<uint32_t, std::vector<VkBuffer>>> uniformBuffers;
	std::unordered_map<std::string, std::pair<uint32_t, std::vector<VkDeviceMemory>>> uniformBuffersMemories;
	std::unordered_map<std::string, std::pair<uint32_t, UniformType>> uniformTypes;

	//Outside data
	VKDevice* device;
	VKPhysicalDevice* physicalDevice;
	VKSwapchain* swapchain;
	VKPipeline* pipeline;
};

#endif