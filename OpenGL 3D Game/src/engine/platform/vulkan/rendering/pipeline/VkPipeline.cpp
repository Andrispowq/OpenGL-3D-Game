#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VkPipeline.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"
#include "engine/platform/vulkan/rendering/shaders/VkShader.h"
#include "engine/platform/vulkan/framework/context/VKContext.h"

VKPipeline::VKPipeline(Shader* shader)
	: Pipeline(shader)
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

	VKSwapchain* swapchain = static_cast<VKSwapchain*>(window->GetSwapchain());
	VKContext* context = static_cast<VKContext*>(window->GetContext());

	this->surface = &context->GetSurface();
	this->physicalDevice = (VKPhysicalDevice*) context->GetPhysicalDevice();
	this->device = (VKDevice*) context->GetDevice();

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