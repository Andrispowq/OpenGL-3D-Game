#ifndef VK_PIPELINE_H
#define VK_PIPELINE_H

#include <vulkan/vulkan.h>

#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"

class VKPipeline : public Pipeline
{
public:
	~VKPipeline();

	void CreatePipeline() override;

	void BindPipeline() const override;
	void UnbindPipeline() const override;

	void DestroyPipeline() override;

	void SetImageFormat(VkFormat* imageFormat) { this->imageFormat = imageFormat; }
	void SetSurface(VkSurfaceKHR* surface) { this->surface = surface; }
	void SetPhysicalDevice(VkPhysicalDevice* physicalDevice) { this->physicalDevice = physicalDevice; }
	void SetLogicalDevice(VkDevice* device) { this->logicalDevice = logicalDevice; }
	void SetShaderStageCreateInfos(VkPipelineShaderStageCreateInfo** shaderStageCreateInfos) { this->shaderStageCreateInfos = shaderStageCreateInfos; }
	void SetSwapChainImageViews(std::vector<VkImageView>* swapChainImageViews) { this->swapChainImageViews = swapChainImageViews; }
	void SetSwapChainExtent(VkExtent2D* swapChainExtent) { this->swapChainExtent = swapChainExtent; }
private:
	VkFormat* imageFormat;
	VkSurfaceKHR* surface;
	VkPhysicalDevice* physicalDevice;
	VkDevice* logicalDevice;
	VkPipelineShaderStageCreateInfo** shaderStageCreateInfos;
	std::vector<VkImageView>* swapChainImageViews;
	VkExtent2D* swapChainExtent;

	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	std::vector<> commandBuffers;
};

#endif