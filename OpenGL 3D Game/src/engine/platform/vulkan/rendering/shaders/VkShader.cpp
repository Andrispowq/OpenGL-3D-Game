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
			PR_LOG_ERROR("Unable to load shader: %s\n", path.c_str());
		}

		return { -1 };
	}
};

VKShader::VKShader(Context* context, Swapchain* swapchain, const std::vector<char>* files, uint32_t length)
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
	else if (length == 5) //Vertex + tessellation control + tessellation evaluation + geomtry + fragment shader
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

	VKSwapchain* vkSwapchain = (VKSwapchain*)swapchain;

	this->numBuffers = (uint32_t)vkSwapchain->getSwapchainImages().size();

	this->swapchain = vkSwapchain;

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

	//Pipeline layout, set here because it's shader dependent
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create pipeline layout!\n");
	}
}

VKShader::VKShader(Context* context, Swapchain* swapchain)
{
	modules = new VkShaderModule[5];
	shaderStages = new VkPipelineShaderStageCreateInfo[5];

	this->physicalDevice = (VKPhysicalDevice*) context->GetPhysicalDevice();
	this->device = (VKDevice*) context->GetDevice();
	counter = 0;

	VKSwapchain* vkSwapchain = (VKSwapchain*)swapchain;

	this->numBuffers = (uint32_t) vkSwapchain->getSwapchainImages().size();

	this->swapchain = vkSwapchain;

	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = numBuffers;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = numBuffers;

	if (vkCreateDescriptorPool(device->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create descriptor pool!\n");
	}

	//Pipeline layout, set here because it's shader dependent
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create pipeline layout!\n");
	}
}

VKShader::~VKShader()
{
	vkDestroyPipelineLayout(device->GetDevice(), pipelineLayout, nullptr);

	for (uint32_t instance = 0; instance < instance_counter; instance++)
	{
		for (const auto& set : descriptorSetLayouts)
		{
			vkDestroyDescriptorSetLayout(device->GetDevice(), set.second[instance], nullptr);
		}

		for (const auto& buffer : uniformBuffers)
		{
			for (size_t i = 0; i < numBuffers; i++)
			{
				vkDestroyBuffer(device->GetDevice(), buffer.second.first[i][instance], nullptr);
			}
		}

		for (const auto& memory : uniformBuffersMemories)
		{
			for (size_t i = 0; i < numBuffers; i++)
			{
				vkFreeMemory(device->GetDevice(), memory.second.first[i][instance], nullptr);
			}
		}
	}

	for (size_t i = 0; i < counter; i++)
	{
		vkDestroyShaderModule(device->GetDevice(), modules[i], nullptr);
	}

	vkDestroyDescriptorPool(device->GetDevice(), descriptorPool, nullptr);

	delete[] modules;
	delete[] shaderStages;
}

/*
	We only add every uniform once, so we could use the index 0 everywhere, but upon creation the instance_counter is 0, so we'll use that
*/
bool VKShader::AddUniform(const std::string& name, uint32_t stages, UniformType type, uint32_t set, uint32_t binding, size_t size, Texture* texture)
{
	uint32_t flags = GetShaderStages(stages);
	VkDescriptorType bindingType = GetDescriptorType(type);

	VkDescriptorSetLayout _DescriptorSetLayout;
	std::vector<VkDescriptorSet> _DescriptorSets;

	auto setIndex = descriptorSets.find(set);
	auto layoutIndex = descriptorSetLayouts.find(set);
	if  (setIndex != descriptorSets.end() && layoutIndex != descriptorSetLayouts.end())
	{
		_DescriptorSetLayout = descriptorSetLayouts.at(set)[instance_counter];
		_DescriptorSets = descriptorSets.at(set)[instance_counter];

		vkDestroyDescriptorSetLayout(device->GetDevice(), _DescriptorSetLayout, nullptr);
		vkFreeDescriptorSets(device->GetDevice(), descriptorPool, numBuffers, _DescriptorSets.data());

		descriptorSets.erase(descriptorSets.find(set));
		descriptorSetLayouts.erase(descriptorSetLayouts.find(set));
	}
	else
	{
		numSets++;
	}

	vkDestroyPipelineLayout(device->GetDevice(), pipelineLayout, nullptr);
	vkDestroyDescriptorPool(device->GetDevice(), descriptorPool, nullptr);
	numDescriptors++;

	//Recreate the descriptor pool
	if (descriptorSizes.size() == 0 || descriptorSizes.find(type) == descriptorSizes.end())
	{
		VkDescriptorPoolSize poolSize = {};
		poolSize.type = bindingType;
		poolSize.descriptorCount = 0; //We create numBuffer descriptors for each set we utilise

		descriptorSizes.insert(std::make_pair(type, poolSize));
	}

	std::vector<VkDescriptorPoolSize> poolSizes;
	poolSizes.reserve(descriptorSizes.size());

	for (auto& plSize : descriptorSizes)
	{
		if (plSize.first == type)
		{
			plSize.second.descriptorCount += numBuffers; //We create numBuffer descriptors for each set we utilise
		}

		poolSizes.push_back(plSize.second);
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = (uint32_t) poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = numSets * numBuffers;

	if (vkCreateDescriptorPool(device->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create descriptor pool!\n");
	}

	//We build this UBO
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = bindingType;
	layoutBinding.descriptorCount = 1;
	layoutBinding.binding = (uint32_t) binding;
	layoutBinding.stageFlags = flags;
	layoutBinding.pImmutableSamplers = nullptr; // Optional

	std::pair<uint32_t, VkDescriptorSetLayoutBinding> bind = { binding, layoutBinding };
	setBindings.insert(std::make_pair(name, std::make_pair(set, binding)));
	
	auto& listOfSetBindings = bindings[set];
	listOfSetBindings.insert(bind);

	//We create the descriptor set layout
	std::vector<VkDescriptorSetLayoutBinding> listOfBindings;
	listOfBindings.reserve(listOfSetBindings.size());
	
	for (auto& element : listOfSetBindings)
	{
		listOfBindings.push_back(element.second);
	}

	VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
	setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setLayoutInfo.bindingCount = (uint32_t) listOfBindings.size();
	setLayoutInfo.pBindings = listOfBindings.data();

	if (vkCreateDescriptorSetLayout(device->GetDevice(), &setLayoutInfo, nullptr, &_DescriptorSetLayout) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create descriptor set layout!\n");
		return false;
	}

	std::vector<VkDescriptorSetLayout> _DSetLayout = { _DescriptorSetLayout };
	descriptorSetLayouts.insert(std::make_pair(set, _DSetLayout));

	//We create the pipeline layout
	std::vector<VkDescriptorSetLayout> listOfLayoutSets;
	listOfLayoutSets.reserve(descriptorSetLayouts.size());

	for (const auto& _Layout : descriptorSetLayouts)
	{
		listOfLayoutSets.push_back(_Layout.second[instance_counter]);
	}

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = (uint32_t) listOfLayoutSets.size();
	pipelineLayoutInfo.pSetLayouts = listOfLayoutSets.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create pipeline layout!\n");
	}

	//Allocation
	if (type == UniformBuffer)
	{
		CreateUniformBuffer(size, name);
	}
	else if (type == CombinedImageSampler && texture != nullptr)
	{
		VKTexture* tex = (VKTexture*)texture;

		std::vector<std::vector<VkImageView>> views(numBuffers, { tex->GetTextureImageView() });
		imageViews.insert(std::make_pair(name, views));

		std::vector< std::vector<VkSampler>> sampler(numBuffers, { tex->GetTextureSampler() });
		samplers.insert(std::make_pair(name, sampler));
	}
	
	uniformTypes.insert(std::make_pair(name, type));

	//Create the descriptor sets
	std::vector<VkDescriptorSetLayout> layouts(numBuffers, _DescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = numBuffers;
	allocInfo.pSetLayouts = layouts.data();

	//We can now allocate the new buffers
	_DescriptorSets.resize(numBuffers);
	if (vkAllocateDescriptorSets(device->GetDevice(), &allocInfo, _DescriptorSets.data()) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to allocate descriptor sets!\n");
		return false;
	}

	std::vector<std::vector<VkDescriptorSet>> _DSets = { _DescriptorSets };
	descriptorSets.insert(std::make_pair(set, _DSets));

	/*for (const auto& _set : descriptorSets)
	{
		PR_LOG_MESSAGE("Descriptor set %u, ID: %p\n", _set.first, _set.second[0]);
		PR_LOG_MESSAGE("Descriptor set %u, ID: %p\n", _set.first, _set.second[1]);
		PR_LOG_MESSAGE("Descriptor set %u, ID: %p\n", _set.first, _set.second[2]);
	}*/

	std::unordered_map<uint32_t, std::pair<std::vector<std::vector<VkBuffer>>, size_t>> uBuffers;

	for (const auto& buff : uniformBuffers)
	{
		if (setBindings[buff.first].first == set)
		{
			uBuffers.insert(std::make_pair(setBindings[buff.first].second, buff.second));
		}
	}

	for (const auto& buffer : uBuffers)
	{
		for (size_t i = 0; i < numBuffers; i++)
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = buffer.second.first[instance_counter][i];
			bufferInfo.offset = 0;
			bufferInfo.range = buffer.second.second;

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = descriptorSets[set][instance_counter][i];
			descriptorWrite.dstBinding = buffer.first;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pTexelBufferView = nullptr; // Optional

			vkUpdateDescriptorSets(device->GetDevice(), 1, &descriptorWrite, 0, nullptr);
		}
	}

	return true;
}

VkDescriptorType VKShader::GetDescriptorType(UniformType type) const
{
	VkDescriptorType bindingType;

	if (type == Sampler) bindingType = VK_DESCRIPTOR_TYPE_SAMPLER;
	if (type == CombinedImageSampler) bindingType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	if (type == SampledImage) bindingType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	if (type == StorageImage) bindingType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	if (type == UniformTexelBuffer) bindingType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
	if (type == StorageTexelBuffer) bindingType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
	if (type == UniformBuffer) bindingType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	if (type == StorageBuffer) bindingType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	if (type == UniformBufferDynamic) bindingType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	if (type == StorageBufferDynamic) bindingType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
	if (type == InputAttachment) bindingType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	if (type == InlineUniformBlock_EXT) bindingType = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT;
	if (type == AccelerationStructure_NV) bindingType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;

	return bindingType;
}

uint32_t VKShader::GetShaderStages(uint32_t stages) const
{
	uint32_t flags = 0;

	if (stages & VERTEX_SHADER) flags = flags | VK_SHADER_STAGE_VERTEX_BIT;
	if (stages & TESSELLATION_CONTROL_SHADER) flags = flags | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	if (stages & TESSELLATION_EVALUATION_SHADER) flags = flags | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	if (stages & GEOMETRY_SHADER) flags = flags | VK_SHADER_STAGE_GEOMETRY_BIT;
	if (stages & FRAGMENT_SHADER) flags = flags | VK_SHADER_STAGE_FRAGMENT_BIT;
	if (stages & RAY_GENERATION_SHADER_NV) flags = flags | VK_SHADER_STAGE_RAYGEN_BIT_NV;
	if (stages & RAY_HIT_SHADER_NV) flags = flags | VK_SHADER_STAGE_ANY_HIT_BIT_NV;
	if (stages & RAY_CLOSEST_HIT_SHADER_NV) flags = flags | VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV;
	if (stages & RAY_CALLABLE_SHADER_NV) flags = flags | VK_SHADER_STAGE_CALLABLE_BIT_NV;
	if (stages & RAY_MISS_SHADER_NV) flags = flags | VK_SHADER_STAGE_MISS_BIT_NV;
	if (stages & RAY_INTERSECTION_SHADER_NV) flags = flags | VK_SHADER_STAGE_INTERSECTION_BIT_NV;
	if (stages & TASK_SHADER_NV) flags = flags | VK_SHADER_STAGE_TASK_BIT_NV;
	if (stages & MESH_SHADER_NV) flags = flags | VK_SHADER_STAGE_MESH_BIT_NV;

	return flags;
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
	case TASK_SHADER_NV:
		flag = VK_SHADER_STAGE_TASK_BIT_NV;
		break;
	case MESH_SHADER_NV:
		flag = VK_SHADER_STAGE_MESH_BIT_NV;
		break;
	case RAY_GENERATION_SHADER_NV:
		flag = VK_SHADER_STAGE_RAYGEN_BIT_NV;
		break;
	case RAY_HIT_SHADER_NV:
		flag = VK_SHADER_STAGE_ANY_HIT_BIT_NV;
		break;
	case RAY_CLOSEST_HIT_SHADER_NV:
		flag = VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV;
		break;
	case RAY_MISS_SHADER_NV:
		flag = VK_SHADER_STAGE_MISS_BIT_NV;
		break;
	case RAY_INTERSECTION_SHADER_NV:
		flag = VK_SHADER_STAGE_INTERSECTION_BIT_NV;
		break;
	case RAY_CALLABLE_SHADER_NV:
		flag = VK_SHADER_STAGE_CALLABLE_BIT_NV;
		break;
	default:
		return false;
	}

	CreateStageInfo(info, module, "main", flag);

	modules[counter] = module;
	shaderStages[counter++] = info;

	return true;
}

void VKShader::BindSet(void* commandBuffer, uint32_t set, uint32_t instance_index) const
{
	if (set >= descriptorSets.size())
		PR_LOG_RUNTIME_ERROR("Tried to bind set %i, but only 0 to %i sets are valid!\n", set, descriptorSets.size());

	VKCommandBuffer* vkcmdbuff = reinterpret_cast<VKCommandBuffer*>(commandBuffer);
	
	vkCmdBindDescriptorSets(vkcmdbuff->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets.at(set).at(instance_index).at(swapchain->GetAquiredImageIndex()), 0, nullptr);
}

void VKShader::SetUniformi(const std::string& name, int value, size_t offset, uint32_t instance_index) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).first[instance_index][swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, offset, sizeof(int), 0, &data);
	memcpy(data, &value, sizeof(int));
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniformf(const std::string& name, float value, size_t offset, uint32_t instance_index) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).first[instance_index][swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, offset, sizeof(float), 0, &data);
	memcpy(data, &value, sizeof(float));
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniform(const std::string& name, const Vector2f& value, size_t offset, uint32_t instance_index) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).first[instance_index][swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, offset, Vector2f::size(), 0, &data);
	memcpy(data, &value, Vector2f::size());
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniform(const std::string& name, const Vector3f& value, size_t offset, uint32_t instance_index) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).first[instance_index][swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, offset, Vector3f::size(), 0, &data);
	memcpy(data, &value, Vector3f::size());
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniform(const std::string& name, const Vector4f& value, size_t offset, uint32_t instance_index) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).first[instance_index][swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, offset, Vector4f::size(), 0, &data);
	memcpy(data, &value, Vector4f::size());
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniform(const std::string& name, const Matrix4f& value, size_t offset, uint32_t instance_index) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).first[instance_index][swapchain->GetAquiredImageIndex()];

	float* dataF = value.m;

	vkMapMemory(device->GetDevice(), mem, offset, sizeof(float) * 16, 0, &data);
	memcpy(data, dataF, sizeof(float) * 16);
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetTexture(const std::string& name, Texture* value, uint32_t instance_index) const
{
	std::pair<uint32_t, uint32_t> setBind = setBindings.at(name);

	VKTexture* tex = (VKTexture*)value;

	for (size_t i = 0; i < numBuffers; i++)
	{
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = tex->GetTextureImageView();
		imageInfo.sampler = tex->GetTextureSampler();

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets.at(setBind.first)[instance_index][i];
		descriptorWrite.dstBinding = setBind.second;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;
		descriptorWrite.pBufferInfo = nullptr;
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(device->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}

void VKShader::SetUniform(const std::string& name, const void* value, size_t size, size_t offset, uint32_t instance_index) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).first[instance_index][swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, offset, size, 0, &data);
	memcpy(data, value, size);
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::RegisterInstance()
{
	instance_counter++;
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

void VKShader::CreateUniformBuffer(VkDeviceSize bufferSize, const std::string& name)
{
	std::vector<VkBuffer> ubos(numBuffers);
	std::vector<VkDeviceMemory> uboMemories(numBuffers);

	for (size_t i = 0; i < numBuffers; i++)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &ubos[i]) != VK_SUCCESS)
		{
			PR_LOG_RUNTIME_ERROR("Failed to create buffer!\n");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device->GetDevice(), ubos[i], &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VKUtil::FindMemoryType(memRequirements.memoryTypeBits, physicalDevice->GetPhysicalDevice(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &uboMemories[i]) != VK_SUCCESS)
		{
			PR_LOG_RUNTIME_ERROR("Failed to allocate buffer memory!\n");
		}

		vkBindBufferMemory(device->GetDevice(), ubos[i], uboMemories[i], 0);
	}

	std::vector<std::vector<VkBuffer>> _ubos = { ubos };
	std::vector<std::vector<VkDeviceMemory>> _uboMemories = { uboMemories };
	uniformBuffers.insert(std::make_pair(name, std::make_pair(_ubos, bufferSize)));
	uniformBuffersMemories.insert(std::make_pair(name, std::make_pair(_uboMemories, bufferSize)));
}