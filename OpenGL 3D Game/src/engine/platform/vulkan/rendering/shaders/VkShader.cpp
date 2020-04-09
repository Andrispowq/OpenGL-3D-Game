#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"
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

	this->numBuffers = (uint32_t)vkSwapchain->GetSwapchainImages().size();

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

	this->numBuffers = (uint32_t) vkSwapchain->GetSwapchainImages().size();

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

	for (const auto& set : descriptorSetLayouts)
	{
		vkDestroyDescriptorSetLayout(device->GetDevice(), set.second, nullptr);
	}

	for (const auto& buffer : uniformBuffers)
	{
		for (size_t i = 0; i < numBuffers; i++)
		{
			vkDestroyBuffer(device->GetDevice(), buffer.second.second[i], nullptr);
		}
	}
	
	for (const auto& memory : uniformBuffersMemories)
	{
		for (size_t i = 0; i < numBuffers; i++)
		{
			vkFreeMemory(device->GetDevice(), memory.second.second[i], nullptr);
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

bool VKShader::AddUniform(const std::string& name, ShaderType stages, uint32_t binding, uint32_t set, size_t size)
{
	VkDescriptorSetLayout _DescriptorSetLayout;
	std::vector<VkDescriptorSet> _DescriptorSets;

	auto setIndex = descriptorSets.find(set);
	auto layoutIndex = descriptorSetLayouts.find(set);
	if  (setIndex != descriptorSets.end() && layoutIndex != descriptorSetLayouts.end())
	{
		_DescriptorSetLayout = descriptorSetLayouts.at((uint32_t) set);
		_DescriptorSets = descriptorSets.at((uint32_t) set);

		vkDestroyDescriptorSetLayout(device->GetDevice(), _DescriptorSetLayout, nullptr);
		//vkFreeDescriptorSets(device->GetDevice(), descriptorPool, numBuffers, _DescriptorSets.data());

		descriptorSets.erase(descriptorSets.find((uint32_t) set));
		descriptorSetLayouts.erase(descriptorSetLayouts.find((uint32_t)set));
	}
	else
	{
		numSets++;
	}

	vkDestroyPipelineLayout(device->GetDevice(), pipelineLayout, nullptr);
	vkDestroyDescriptorPool(device->GetDevice(), descriptorPool, nullptr);
	numDescriptors++;

	//Recreate the descriptor pool
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = numDescriptors * numBuffers; //We create numBuffer sets for each set we utilise

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = numSets * numBuffers;

	if (vkCreateDescriptorPool(device->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create descriptor pool!\n");
	}

	//We build this UBO
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

	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.binding = (uint32_t) binding;
	uboLayoutBinding.stageFlags = flags;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	std::pair<uint32_t, VkDescriptorSetLayoutBinding> bind = { set, uboLayoutBinding };

	bindings.insert(std::make_pair(name, bind));

	//We create the descriptor set layout
	std::vector<VkDescriptorSetLayoutBinding> listOfBindings;

	for (const auto& binding : bindings)
	{
		if(binding.second.first == set)
			listOfBindings.push_back(binding.second.second);
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = (uint32_t) listOfBindings.size();
	layoutInfo.pBindings = listOfBindings.data();

	if (vkCreateDescriptorSetLayout(device->GetDevice(), &layoutInfo, nullptr, &_DescriptorSetLayout) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create descriptor set layout!\n");
		return false;
	}

	descriptorSetLayouts.insert(std::make_pair(set, _DescriptorSetLayout));

	//We create the pipeline layout
	std::vector<VkDescriptorSetLayout> listOfLayoutSets;
	listOfLayoutSets.reserve(descriptorSetLayouts.size());

	for (const auto& _Layout : descriptorSetLayouts)
	{
		listOfLayoutSets.push_back(_Layout.second);
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

	//We have to recreate the pipeline because an important component has changed
	//pipeline->RecreatePipeline();

	//Allocate uniform buffers
	VkDeviceSize bufferSize = size;

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
			return false;
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
			return false;
		}

		vkBindBufferMemory(device->GetDevice(), ubos[i], uboMemories[i], 0);
	}

	std::pair<uint32_t, std::vector<VkBuffer>> setUBO = { set, ubos };
	std::pair<uint32_t, std::vector<VkDeviceMemory>> setUBOMems = { set, uboMemories };

	uniformBuffers.insert(std::make_pair(name, setUBO));
	uniformBuffersMemories.insert(std::make_pair(name, setUBOMems));

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

	descriptorSets.insert(std::make_pair(set, _DescriptorSets));

	for (size_t i = 0; i < numBuffers; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[name].second[i];
		bufferInfo.offset = 0;
		bufferInfo.range = bufferSize;
		
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[(uint32_t) set][i];
		descriptorWrite.dstBinding = (uint32_t) binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	for (size_t i = 0; i < numBuffers; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[name].second[i];
		bufferInfo.offset = 0;
		bufferInfo.range = bufferSize;

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[(uint32_t)set][i];
		descriptorWrite.dstBinding = (uint32_t)binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	return true;
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

void VKShader::Bind(void* commandBuffer) const
{
	std::vector<VkDescriptorSet> sets;
	sets.reserve(descriptorSets.size());
	uint8_t count = 0;

	for (const auto& set : descriptorSets)
	{
		sets.push_back(set.second[swapchain->GetAquiredImageIndex()]);
		count++;
	}

	if (count == 0)
		return;

	VKCommandBuffer* vkcmdbuff = reinterpret_cast<VKCommandBuffer*>(commandBuffer);

	vkCmdBindDescriptorSets(vkcmdbuff->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, count, sets.data(), 0, nullptr);
}

void VKShader::SetUniformi(const std::string& name, int value) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).second[swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, 0, sizeof(int), 0, &data);
	memcpy(data, &value, sizeof(int));
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniformf(const std::string& name, float value) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).second[swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, 0, sizeof(float), 0, &data);
	memcpy(data, &value, sizeof(float));
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniform(const std::string& name, const Vector2f& value) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).second[swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, 0, sizeof(Vector2f), 0, &data);
	memcpy(data, &value, sizeof(Vector2f));
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniform(const std::string& name, const Vector3f& value) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).second[swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, 0, sizeof(Vector3f), 0, &data);
	memcpy(data, &value, sizeof(Vector3f));
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniform(const std::string& name, const Vector4f& value) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).second[swapchain->GetAquiredImageIndex()];

	vkMapMemory(device->GetDevice(), mem, 0, sizeof(Vector4f), 0, &data);
	memcpy(data, &value, sizeof(Vector4f));
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::SetUniform(const std::string& name, const Matrix4f& value) const
{
	void* data;
	const VkDeviceMemory& mem = uniformBuffersMemories.at(name).second[swapchain->GetAquiredImageIndex()];

	float* dataF = value.GetData();

	value.print();

	vkMapMemory(device->GetDevice(), mem, 0, sizeof(float) * 16, 0, &data);
	memcpy(data, dataF, sizeof(float) * 16);
	vkUnmapMemory(device->GetDevice(), mem);
}

void VKShader::UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const
{
	SetUniform("ubo1.model", object->GetWorldTransform()->getTransformationMatrix());
	SetUniform("ubo2.view", camera->GetViewMatrix());
	SetUniform("ubo3.proj", camera->GetProjectionMatrix());
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