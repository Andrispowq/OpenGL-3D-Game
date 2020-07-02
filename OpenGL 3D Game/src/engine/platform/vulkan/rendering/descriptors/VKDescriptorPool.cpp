#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKDescriptorPool.h"

VKDescriptorPool::VKDescriptorPool(VKDevice* device, VKSwapchain* swapchain)
	: device(device), swapchain(swapchain)
{

}

VKDescriptorPool::~VKDescriptorPool()
{
	vkDestroyDescriptorPool(device->GetDevice(), pool, nullptr);
}

void VKDescriptorPool::addSet(const VKDescriptorSet& set)
{
	sets.push_back(set);
}

void VKDescriptorPool::addSet()
{
	VKDescriptorSet set(device, sets.size());
	sets.push_back(set);
}

void VKDescriptorPool::finalize(VkPipelineLayout& layout)
{
	std::vector<VkDescriptorSetLayout> layouts;
	std::vector<VkDescriptorSet> _sets;
	layouts.reserve(sets.size());
	_sets.reserve(sets.size());

	for (auto& set : sets)
	{
		set.finalize();
		layouts.push_back(set.getLayout());
		_sets.push_back(set.getSet());
	}

	VkPipelineLayoutCreateInfo _layoutCreateInfo = {};
	_layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	_layoutCreateInfo.setLayoutCount = layouts.size();
	_layoutCreateInfo.pSetLayouts = layouts.data();
	_layoutCreateInfo.pushConstantRangeCount = 0; // For now
	_layoutCreateInfo.pPushConstantRanges = nullptr; // For now

	if (!vkCreatePipelineLayout(device->GetDevice(), &_layoutCreateInfo, nullptr, &layout) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create the pipeline layout in VkDescriptorPool::finalize(void)!\n");
	}

	std::vector<VkDescriptorPoolSize> _poolSizes(2);

	uint32_t uniformCount = 0;
	uint32_t textureCount = 0;

	for (auto& set : sets)
	{
		for (auto& binding : set.getBindings())
		{
			if (binding.getBuffer() == nullptr) //It's either a uniform buffer or a texture descriptor now
				textureCount++;
			else
				uniformCount++;
		}
	}

	_poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	_poolSizes[0].descriptorCount = uniformCount * swapchain->getSwapchainImages().size();

	_poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	_poolSizes[1].descriptorCount = textureCount * swapchain->getSwapchainImages().size();

	VkDescriptorPoolCreateInfo _poolCreateInfo = {};
	_poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	_poolCreateInfo.poolSizeCount = _poolSizes.size();
	_poolCreateInfo.pPoolSizes = _poolSizes.data();
	_poolCreateInfo.maxSets = sets.size() * swapchain->getSwapchainImages().size();
	_poolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	if (vkCreateDescriptorPool(device->GetDevice(), &_poolCreateInfo, nullptr, &pool) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create the descriptor pool in VkDescriptorPool::finalize(void)!\n");
	}

	VkDescriptorSetAllocateInfo _setAllocInfo = {};
	_setAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	_setAllocInfo.descriptorPool = pool;
	_setAllocInfo.descriptorSetCount = sets.size() * swapchain->getSwapchainImages().size(); //We create a set for every swapchain image for easier synchronization
	_setAllocInfo.pSetLayouts = layouts.data();
	_setAllocInfo.pNext = nullptr;

	if (!vkAllocateDescriptorSets(device->GetDevice(), &_setAllocInfo, _sets.data()) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to allocate %i descriptor sets from Descriptor Pool  at location %p in function VKDescriptorPool::finalize(void)!\n", _setAllocInfo.descriptorSetCount, &pool);
	}
	
	//Updating the descriptors is the last steps
}
