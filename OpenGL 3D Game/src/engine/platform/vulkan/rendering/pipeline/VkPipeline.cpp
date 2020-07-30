#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VkPipeline.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"
#include "engine/platform/vulkan/rendering/shaders/VkShader.h"
#include "engine/platform/vulkan/framework/context/VKContext.h"

VKPipeline::VKPipeline(AssetManager* manager, size_t shaderID)
	: Pipeline(manager, shaderID)
{
}

VKPipeline::~VKPipeline()
{
	if(exists)
		DestroyPipeline();
}

void VKPipeline::CreatePipeline(Window* window)
{
	exists = true;

	VKSwapchain* swapchain = static_cast<VKSwapchain*>(window->getSwapchain());
	VKContext* context = static_cast<VKContext*>(window->getContext());

	this->surface = &context->getSurface();
	this->physicalDevice = (VKPhysicalDevice*) context->getPhysicalDevice();
	this->device = (VKDevice*) context->getDevice();

	this->window = window;
	this->swapchain = swapchain;
}

void VKPipeline::RecreatePipeline()
{
}

void VKPipeline::BindPipeline() const
{
}

void VKPipeline::RenderPipeline() const
{
}

void VKPipeline::UnbindPipeline() const
{
}

void VKPipeline::DestroyPipeline()
{
	exists = false;
}