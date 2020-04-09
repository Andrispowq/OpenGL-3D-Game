#ifndef VK_PIPELINE_H
#define VK_PIPELINE_H

#include <vulkan/vulkan.h>

#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"

#include "engine/platform/vulkan/rendering/command/VKCommandPool.h"
#include "engine/platform/vulkan/rendering/renderpass/VKRenderpass.h"
#include "engine/platform/vulkan/rendering/framebuffer/VKFramebuffer.h"
#include "engine/platform/vulkan/rendering/synchronization/VKSemaphore.h"
#include "VKGraphicsPipeline.h"

#include "engine/platform/vulkan/framework/surface/VKSurface.h"
#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"

class VKPipeline : public Pipeline
{
public:
	VKPipeline(Shader* shader);
	virtual ~VKPipeline();

	virtual void CreatePipeline(Window* window, MeshVBO* vbo) override;

	virtual void BindPipeline() override;
	virtual void UnbindPipeline() const override;

	virtual void DestroyPipeline() override;

	virtual void RecreatePipeline();

	virtual void* GetDrawCommandBuffer() const override { return drawCommandBuffer; }
	virtual void* GetGraphicsPipeline() const override { return &graphicsPipeline->GetGraphicsPipeline(); }
private:
	VkPipelineShaderStageCreateInfo** shaderStageCreateInfos;

	VKSurface* surface;
	VKPhysicalDevice* physicalDevice;
	VKDevice* device;

	VKSwapchain* swapchain;
	VkFormat* imageFormat;
	VkExtent2D* swapchainExtent;
	std::vector<VkImageView> swapchainImageViews;

	VKCommandPool* commandPool;

	VKMeshVBO* vbo;

	Window* window;

	VKRenderpass* renderpass;
	VKGraphicsPipeline* graphicsPipeline;
	std::vector<VKFramebuffer*> swapchainFramebuffers;
	VkCommandBuffer* drawCommandBuffer;

	bool exist = false;
};

#endif