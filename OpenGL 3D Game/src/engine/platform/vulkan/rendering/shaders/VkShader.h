#ifndef VK_SHADER_H
#define VK_SHADER_H

#include "engine/prehistoric/common/rendering/shaders/Shader.h"

#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"
#include "engine/platform/vulkan/framework/device/VKDevice.h"

#include "engine/prehistoric/core/gameObject/GameObject.h"

struct UniformBufferObject
{
	Matrix4f transformation;
	Matrix4f view;
	Matrix4f projection;
};

class VKShader : public Shader
{
public:
	VKShader(Context* context, const std::vector<char>* files, uint32_t length);
	VKShader(Context* context);

	virtual ~VKShader();

	void CreateShader(VKSwapchain* swapchain);

	virtual void Bind(void* commandBuffer) const override;
	virtual void Unbind() const override {}

	virtual bool AddUniform(const std::string& name) override { return true; }
	virtual bool AddUniformBlock(const std::string& name) override { return true; }

	virtual bool AddShader(const std::vector<char>& code, ShaderType type) override;
	virtual bool CompileShader() const override { return true; }

	//Uniform handling
	virtual void SetUniformi(const std::string& name, int value) const override {}
	virtual void SetUniformf(const std::string& name, float value) const override {}

	virtual void SetUniform(const std::string& name, const Vector2f& value) const override {}
	virtual void SetUniform(const std::string& name, const Vector3f& value) const override {}
	virtual void SetUniform(const std::string& name, const Vector4f& value) const override {}
	virtual void SetUniform(const std::string& name, const Matrix4f& value) const override {}

	virtual void BindUniformBlock(const std::string& name, uint32_t binding) const override {}

	virtual void UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const;

	VkPipelineShaderStageCreateInfo* GetShaderStages() { return shaderStages; }
	VkPipelineLayout& GetPipelineLayout() { return pipelineLayout; }
private:
	bool AddShader(const std::vector<char>& code, VkShaderStageFlagBits type);
	VkShaderModule CreateShaderModule(const std::vector<char>& code) const;
	void CreateStageInfo(VkPipelineShaderStageCreateInfo& info, VkShaderModule module, const char* main, VkShaderStageFlagBits type);

	VkShaderModule* modules;
	VkPipelineShaderStageCreateInfo* shaderStages;

	VkPipelineLayout pipelineLayout;

	std::vector<VkDescriptorSetLayoutBinding> bindings; 

	VkDescriptorSetLayout descriptorLayout;
	VkDescriptorSet descriptorSet;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool descriptorPool;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	uint32_t numBuffers;

	VKDevice* device;
	VKPhysicalDevice* physicalDevice;
	VKSwapchain* swapchain;
};

#endif