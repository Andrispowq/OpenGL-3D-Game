#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKFramebuffer.h"
#include "engine/platform/vulkan/rendering/renderpass/VKRenderpass.h"

VKFramebuffer::VKFramebuffer(VKRenderpass* renderpass, VkDevice device, VkExtent2D swapchainExtent,
	VkImageView colourAttachment, VkImageView depthAttachment, VkImageView colourAttachmentResolve)
	: device(device)
{
	VkImageView attachments[3] = 
	{
		colourAttachment,
		depthAttachment,
		colourAttachmentResolve
	};

	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderpass->getRenderPass();
	framebufferInfo.attachmentCount = 3;
	framebufferInfo.pAttachments = attachments;
	framebufferInfo.width = swapchainExtent.width;
	framebufferInfo.height = swapchainExtent.height;
	framebufferInfo.layers = 1;

	if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create framebuffer!\n");
	}
}

VKFramebuffer::~VKFramebuffer()
{
	vkDestroyFramebuffer(device, framebuffer, nullptr);
}