#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkShader.h"

namespace ResourceLoader
{
	std::string ResourceLoader::LoadShaderVK(const std::string& path)
	{
		const std::string SHADER_LOC = "res/shaders/";

		std::ifstream file;
		file.open((SHADER_LOC + path).c_str(), std::ios::ate | std::ios::binary);

		std::string output;
		std::string line;

		if (file.is_open()) 
		{
			while (file.good())
			{
				std::getline(file, line);

				output.append(line);
			}
		}
		else
		{
			PR_LOG_ERROR("Unable to load shader: %s\n", path);
		}

		return output;
	}
};

VKShader::VKShader(VkDevice* device)
{
	modules = new VkShaderModule[5];
	shaderStages = new VkPipelineShaderStageCreateInfo[5];

	this->device = device;
}

VKShader::~VKShader()
{
	for (size_t i = 0; i < counter; i++)
	{
		vkDestroyShaderModule(*device, modules[i], nullptr);
	}
}

bool VKShader::AddShader(const std::string& code, VkShaderStageFlagBits type)
{
	CreateShaderModule(code);

	VkPipelineShaderStageCreateInfo info = {};
	CreateStageInfo(info, modules[counter - 1], "main", type);

	return true;
}

bool VKShader::CreateShaderModule(const std::string& code) const
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(*device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to create shader module!");
		return false;
	}

	modules[counter++] = shaderModule;
	
	return true;
}

void VKShader::CreateStageInfo(VkPipelineShaderStageCreateInfo& info, VkShaderModule module, const char* main, VkShaderStageFlagBits type)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.stage = type;
	info.module = module;
	info.pName = main;
}