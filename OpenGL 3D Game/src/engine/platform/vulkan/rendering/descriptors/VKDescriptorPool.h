#ifndef VK_DESCRIPTOR_POOL_H
#define VK_DESCRIPTOR_POOL_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"

#include "VKDescriptorSet.h"

class VKDescriptorPool
{
public:
	VKDescriptorPool(VKDevice* device, VKSwapchain* swapchain);
	virtual ~VKDescriptorPool();

	void addSet(const VKDescriptorSet& set);
	void addSet();

	/*
		This method will create the descriptor pool, allocate the sets, and so on, and the only thing you can change now is the new instances
	*/
	void finalize();

	void registerInstance();

	VKDescriptorSet& getSet(uint32_t set_index) { return sets[set_index]; }
	uint32_t getNumberOfSets() const { return sets.size(); }

	inline VkDescriptorPool& getDescriptorPool() { return pool; }

private:
	VKDevice* device;
	VKSwapchain* swapchain;

	VkDescriptorPool pool;

	uint32_t instance_count = 1; //We assume the instance count to be one when creating the data structure, and then when we recieve an new instance, we create a copy of the sets in the vector
	std::vector<VKDescriptorSet> sets; //First, we allocate the amount of sets as the swapchain images, then extend the vector when recieving the number of instances

	std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> uniformLocations; //We store here which uniform name corresponds to which set (std::pair::first) and
	//which binding (std::pair::second)
};

#endif