#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKDescriptorSet.h"

VKDescriptorSet::~VKDescriptorSet()
{
	vkDestroyDescriptorSetLayout(device->GetDevice(), layout, nullptr);
}

void VKDescriptorSet::addBinding(const VKDescriptorSetBinding& binding)
{
	bindings.push_back(binding);
}

void VKDescriptorSet::finalize()
{
	std::vector<VkDescriptorSetLayoutBinding> _bindings;
	_bindings.reserve(bindings.size());

	for (auto& binding : bindings)
	{
		binding.finalize();
		_bindings.push_back(binding.getBinding());
	}

	VkDescriptorSetLayoutCreateInfo _createInfo = {};
	_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	_createInfo.bindingCount = (uint32_t)_bindings.size();
	_createInfo.pBindings = _bindings.data();

	if (vkCreateDescriptorSetLayout(device->GetDevice(), &_createInfo, nullptr, &layout) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("VKDescriptorSet couldn't create a VkDescriptorSetLayout object!\n");
	}
}
