#ifndef VK_FRAMEBUFFER_H
#define VK_FRAMEBUFFER_H

#include <vulkan/vulkan.h>

class VKRenderpass;

class VKFramebuffer
{
public:
	VKFramebuffer(VKRenderpass* renderpass, VkDevice device, VkExtent2D swapchainExtent, VkImageView colourAttachment, VkImageView depthAttachment, VkImageView colourAttachmentResolve);
	virtual ~VKFramebuffer();

	VkFramebuffer getFramebuffer() const { return framebuffer; }
private:
	VkFramebuffer framebuffer;
	VkDevice device;
};

#endif