#ifndef VK_FRAMEBUFFER_H
#define VK_FRAMEBUFFER_H

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"

class VKRenderpass;

class VKFramebuffer
{
public:
	VKFramebuffer(VKRenderpass& renderpass, VkExtent2D& swapchainExtent, VkImageView& attachment, VkDevice& device);
	virtual ~VKFramebuffer();

	VkFramebuffer GetFramebuffer() const { return framebuffer; }
private:
	VkFramebuffer framebuffer;
	VkImageView* attachments;

	VKRenderpass* renderpass;
	VkExtent2D* swapchainExtent;

	VkDevice* device;
};

#endif