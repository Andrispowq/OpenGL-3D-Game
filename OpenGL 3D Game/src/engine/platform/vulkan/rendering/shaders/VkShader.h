#ifndef VK_SHADER_H
#define VK_SHADER_H

#include "vulkan/vulkan.h"

#include "engine/prehistoric/common/rendering/shaders/Shader.h"

class VKShader : public Shader
{
public:
	VKShader(VkDevice* device, const std::string* files, unsigned int length);
	VKShader(VkDevice* device);

	virtual ~VKShader();
private:
	bool AddShader(const std::string& code, VkShaderStageFlagBits type);
	bool CreateShaderModule(const std::string& code) const;
	void CreateStageInfo(VkPipelineShaderStageCreateInfo& info, VkShaderModule module, const char* main, VkShaderStageFlagBits type);

	VkShaderModule* modules;
	VkPipelineShaderStageCreateInfo* shaderStages;

	VkDevice* device;
};

#endif