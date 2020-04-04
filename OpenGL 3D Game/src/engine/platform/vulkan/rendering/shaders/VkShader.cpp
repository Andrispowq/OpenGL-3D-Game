#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VkShader.h"

namespace ResourceLoader
{
	std::vector<char> ResourceLoader::LoadShaderVK(const std::string& path)
	{
		const std::string SHADER_LOC = "res/shaders/";

		std::ifstream file;
		file.open((SHADER_LOC + path).c_str(), std::ios::ate | std::ios::binary);

		if (file.is_open()) 
		{
			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();
			return buffer;
		}
		else
		{
			PR_LOG_ERROR("Unable to load shader: %s\n", path);
		}

		return { -1 };
	}
};

VKShader::VKShader(Context* context, const std::vector<char>* files, uint32_t length)
{
	modules = new VkShaderModule[5];
	shaderStages = new VkPipelineShaderStageCreateInfo[5];

	this->physicalDevice = (VKPhysicalDevice*) context->GetPhysicalDevice();
	this->device = (VKDevice*) context->GetDevice();
	counter = 0;

	if (length == 1) //Compute shader
	{
		if (!AddShader(files[0], COMPUTE_SHADER))
			PR_LOG_RUNTIME_ERROR("Compute shader couldn't be added\n");
	}
	else if (length == 2) // Vertex + fragment shader
	{
		if (!AddShader(files[0], VERTEX_SHADER))
			PR_LOG_RUNTIME_ERROR("Vertex shader couldn't be added!");
		if (!AddShader(files[1], FRAGMENT_SHADER))
			PR_LOG_RUNTIME_ERROR("Fragment shader couldn't be added!");
	}
	else if (length == 3) //Vertex + geometry + fragment shader
	{
		if (!AddShader(files[0], VERTEX_SHADER))
			PR_LOG_RUNTIME_ERROR("Vertex shader couldn't be added!");
		if (!AddShader(files[1], GEOMETRY_SHADER))
			PR_LOG_RUNTIME_ERROR("Geometry shader couldn't be added!");
		if (!AddShader(files[2], FRAGMENT_SHADER))
			PR_LOG_RUNTIME_ERROR("Fragment shader couldn't be added!");
	}
	else if (length == 5) //Vertex + tess control + tess evaluation + geomtry + fragment shader
	{
		if (!AddShader(files[0], VERTEX_SHADER))
			PR_LOG_RUNTIME_ERROR("Vertex shader couldn't be added!");
		if (!AddShader(files[1], TESSELLATION_CONTROL_SHADER))
			PR_LOG_RUNTIME_ERROR("Tessellation control shader couldn't be added!");
		if (!AddShader(files[2], TESSELLATION_EVALUATION_SHADER))
			PR_LOG_RUNTIME_ERROR("Tessellation evaluation shader couldn't be added!");
		if (!AddShader(files[3], GEOMETRY_SHADER))
			PR_LOG_RUNTIME_ERROR("Geometry shader couldn't be added!");
		if (!AddShader(files[4], FRAGMENT_SHADER))
			PR_LOG_RUNTIME_ERROR("Fragment shader couldn't be added!");
	}
}

VKShader::VKShader(Context* context)
{
	modules = new VkShaderModule[5];
	shaderStages = new VkPipelineShaderStageCreateInfo[5];

	this->physicalDevice = (VKPhysicalDevice*) context->GetPhysicalDevice();
	this->device = (VKDevice*) context->GetDevice();
	counter = 0;
}

VKShader::~VKShader()
{
	vkDestroyPipelineLayout(device->GetDevice(), pipelineLayout, nullptr);

	vkDestroyDescriptorSetLayout(device->GetDevice(), descriptorLayout, nullptr);

	for (size_t i = 0; i < numBuffers; i++) 
	{
		vkDestroyBuffer(device->GetDevice(), uniformBuffers[i], nullptr);
		vkFreeMemory(device->GetDevice(), uniformBuffersMemory[i], nullptr);
	}

	for (size_t i = 0; i < counter; i++)
	{
		vkDestroyShaderModule(device->GetDevice(), modules[i], nullptr);
	}

	vkDestroyDescriptorPool(device->GetDevice(), descriptorPool, nullptr);

	delete[] modules;
	delete[] shaderStages;
}

void VKShader::CreateShader(VKSwapchain* swapchain)
{
	this->numBuffers = swapchain->GetSwapchainImages().size();

	this->swapchain = swapchain;

	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = numBuffers;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = numBuffers;

	if (vkCreateDescriptorPool(device->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create descriptor pool!\n");
	}

	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(device->GetDevice(), &layoutInfo, nullptr, &descriptorLayout) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create descriptor set layout!\n");
	}

	//Pipeline layout, set here because it's shader dependent
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create pipeline layout!\n");
	}

	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(numBuffers);
	uniformBuffersMemory.resize(numBuffers);

	for (size_t i = 0; i < numBuffers; i++)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &uniformBuffers[i]) != VK_SUCCESS)
		{
			PR_LOG_RUNTIME_ERROR("Failed to create buffer!\n");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device->GetDevice(), uniformBuffers[i], &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VKUtil::FindMemoryType(memRequirements.memoryTypeBits, physicalDevice->GetPhysicalDevice(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &uniformBuffersMemory[i]) != VK_SUCCESS)
		{
			PR_LOG_RUNTIME_ERROR("Failed to allocate buffer memory!\n");
		}

		vkBindBufferMemory(device->GetDevice(), uniformBuffers[i], uniformBuffersMemory[i], 0);
	}

	std::vector<VkDescriptorSetLayout> layouts(numBuffers, descriptorLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = numBuffers;
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(numBuffers);
	if (vkAllocateDescriptorSets(device->GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to allocate descriptor sets!\n");
	}

	for (size_t i = 0; i < numBuffers; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}

bool VKShader::AddShader(const std::vector<char>& code, ShaderType type)
{
	VkShaderModule module = CreateShaderModule(code);
	VkPipelineShaderStageCreateInfo info = {};

	VkShaderStageFlagBits flag;

	switch (type)
	{
	case VERTEX_SHADER:
		flag = VK_SHADER_STAGE_VERTEX_BIT;
		break;
	case TESSELLATION_CONTROL_SHADER:
		flag = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		break;
	case TESSELLATION_EVALUATION_SHADER:
		flag = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		break;
	case GEOMETRY_SHADER:
		flag = VK_SHADER_STAGE_GEOMETRY_BIT;
		break;
	case FRAGMENT_SHADER:
		flag = VK_SHADER_STAGE_FRAGMENT_BIT;
		break;
	case COMPUTE_SHADER:
		flag = VK_SHADER_STAGE_COMPUTE_BIT;
		break;
	default:
		return false;
	}

	CreateStageInfo(info, module, "main", flag);

	modules[counter] = module;
	shaderStages[counter++] = info;

	return true;
}

void VKShader::Bind(void* commandBuffer) const
{
	vkCmdBindDescriptorSets(reinterpret_cast<VKCommandBuffer*>(commandBuffer)->GetCommandBuffer(),
		VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[swapchain->GetAquiredImageIndex()], 0, nullptr);
}

void VKShader::UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const
{
	UniformBufferObject ubo = {};
	ubo.transformation = object->GetWorldTransform()->getTransformationMatrix();
	ubo.view = camera->GetViewMatrix();
	ubo.projection = camera->GetProjectionMatrix();

	constexpr size_t MatrixSize = 16 * sizeof(float);

	float* dataf = new float[16 * 3];

	memcpy(&dataf[0], ubo.transformation.GetData(), MatrixSize);
	memcpy(&dataf[16], ubo.view.GetData(), MatrixSize);
	memcpy(&dataf[32], ubo.projection.GetData(), MatrixSize);

	void* data;
	vkMapMemory(device->GetDevice(), uniformBuffersMemory[swapchain->GetAquiredImageIndex()], 0, MatrixSize * 3, 0, &data);
		memcpy(data, dataf, MatrixSize * 3);
	vkUnmapMemory(device->GetDevice(), uniformBuffersMemory[swapchain->GetAquiredImageIndex()]);
}

VkShaderModule VKShader::CreateShaderModule(const std::vector<char>& code) const
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device->GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to create shader module!\n");
		return VK_NULL_HANDLE;
	}
	
	return shaderModule;
}

void VKShader::CreateStageInfo(VkPipelineShaderStageCreateInfo& info, VkShaderModule module, const char* main, VkShaderStageFlagBits type)
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.stage = type;
	info.module = module;
	info.pName = main;
}