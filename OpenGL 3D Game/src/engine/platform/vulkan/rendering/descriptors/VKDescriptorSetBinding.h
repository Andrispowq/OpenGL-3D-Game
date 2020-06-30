#ifndef VK_DESCRIPTOR_SET_BINDING_H
#define VK_DESCRIPTOR_SET_BINDING_H

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/buffer/VKBuffer.h"
#include "engine/platform/vulkan/texture/VkTexture.h"

union DescriptorData
{
	VKBuffer* buffer;
	VKTexture* texture;
};

class VKDescriptorSetBinding
{
public:
	VKDescriptorSetBinding(VkDescriptorSetLayoutBinding _binding) : _binding(_binding) {}
	VKDescriptorSetBinding() : _binding(VkDescriptorSetLayoutBinding{}) {}
	virtual ~VKDescriptorSetBinding() {}

	inline VkDescriptorSetLayoutBinding& getBinding() { return _binding; }

	inline void setData(DescriptorData data) { this->data = data; }
private:
	VkDescriptorSetLayoutBinding _binding;
	DescriptorData data;
};

#endif