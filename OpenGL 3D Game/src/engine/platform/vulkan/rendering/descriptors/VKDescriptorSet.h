#ifndef VK_DESCRIPTOR_SET_H
#define VK_DESCRIPTOR_SET_H

#include <vulkan/vulkan.h>

#include <vector>

#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "VKDescriptorSetBinding.h"

class VKDescriptorSet
{
public:
	VKDescriptorSet(VKDevice* device, uint32_t set_index) : device(device), set_index(set_index), layout(VK_NULL_HANDLE) {}
	virtual ~VKDescriptorSet();

	void addBinding(const VKDescriptorSetBinding& binding);

	void finalize(); //We don't want to add a big overhead by regenerating the set layout everytime we add a binding, so this function should be invoked when we added every descriptor

	inline VkDescriptorSetLayout& getLayout() { return layout; }
	inline VkDescriptorSet& getSet() { return set; }

	inline std::vector<VKDescriptorSetBinding>& getBindings() { return bindings; }
private:
 	VKDevice* device;

	uint32_t set_index;
	VkDescriptorSetLayout layout;

	std::vector<VKDescriptorSetBinding> bindings; //Binding 0's data is at index 0, this should be the case as it makes no sense to have descriptors 2 and 4, but leave out 3. 
	//This implies that we have to add the bindings in sequentual order

	VkDescriptorSet set;
};

#endif