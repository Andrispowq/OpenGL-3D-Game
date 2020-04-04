#ifndef VK_RENDERPASS_H
#define VK_RENDERPASS_H

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/rendering/framebuffer/VKFramebuffer.h"
#include "engine/platform/vulkan/rendering/command/VKCommandBuffer.h"

class VKRenderpass
{
public:
	VKRenderpass(VkDevice& device, VkFormat& imageFormat);
	virtual ~VKRenderpass();

	void BeginRenderpass(VKCommandBuffer& commandBuffer, VkExtent2D& swapchainExtent, VKFramebuffer& framebuffer);
	void EndRenderpass(VKCommandBuffer& commandBuffer);

	VkRenderPass GetRenderPass() const { return renderpass; }
private:
	VkRenderPass renderpass;

	VkDevice* device;
	VkFormat* imageFormat;
};

#endif