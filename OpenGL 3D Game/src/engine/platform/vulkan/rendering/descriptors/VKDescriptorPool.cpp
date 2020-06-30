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

void VKDescriptorPool::finalize()
{
	for (auto& set : sets)
	{
		set.finalize();
	}

	VkDescriptorSetAllocateInfo allocInfo = {};
}
