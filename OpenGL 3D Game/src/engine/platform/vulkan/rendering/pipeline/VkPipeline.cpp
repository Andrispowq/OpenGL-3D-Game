#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VkPipeline.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"
#include "engine/platform/vulkan/rendering/shaders/VkShader.h"
#include "engine/platform/vulkan/framework/context/VKContext.h"

VKPipeline::VKPipeline(Shader* shader, VBO* vbo)
	: Pipeline(shader, vbo)
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

	graphicsPipeline = new VKGraphicsPipeline(*device, (VKShader*)shader, swapchain->getRenderpass(), viewportStart, viewportSize, scissorStart, scissorSize, 
		backfaceCulling, *((VKMeshVBO*)vbo), physicalDevice->getSampleCount());
}

void VKPipeline::RecreatePipeline()
{
	//We delete the old objects
	delete graphicsPipeline;

	//Create new objects
	graphicsPipeline = new VKGraphicsPipeline(*device, (VKShader*)shader, swapchain->getRenderpass(), viewportStart, viewportSize, scissorStart, scissorSize,
		backfaceCulling, *((VKMeshVBO*)vbo), physicalDevice->getSampleCount());
}

void VKPipeline::BindPipeline() const
{
	VKCommandBuffer* buff = (VKCommandBuffer*) swapchain->GetDrawCommandBuffer();

	buff->BindBuffer();

	vbo->Bind(buff);
	shader->Bind(buff);

	swapchain->BeginRenderpass();
	graphicsPipeline->BindGraphicsPipeline(*buff);
}

void VKPipeline::RenderPipeline() const
{
	vbo->Draw(swapchain->GetDrawCommandBuffer());
}

void VKPipeline::UnbindPipeline() const
{
	VKCommandBuffer* buff = (VKCommandBuffer*)swapchain->GetDrawCommandBuffer();

	swapchain->EndRenderpass();

	shader->Unbind();
	vbo->Unbind();

	buff->UnbindBuffer();
}

void VKPipeline::DestroyPipeline()
{
	delete graphicsPipeline;

	exists = false;
}