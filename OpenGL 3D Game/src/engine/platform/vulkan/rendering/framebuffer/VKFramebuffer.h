#ifndef VK_FRAMEBUFFER_H
#define VK_FRAMEBUFFER_H

#include <vulkan/vulkan.h>

class VKRenderpass;

class VKFramebuffer
{
public:
	VKFramebuffer(VKRenderpass& renderpass, VkExtent2D& swapchainExtent, VkImageView& colourAttachment, VkImageView& depthAttachment, VkImageView& colourAttachmentResolve, VkDevice& device);
	virtual ~VKFramebuffer();

	VkFramebuffer GetFramebuffer() const { return framebuffer; }
private:
	VkFramebuffer framebuffer;
	VkDevice device;
};

#endif