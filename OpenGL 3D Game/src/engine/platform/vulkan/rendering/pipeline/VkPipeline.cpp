#include "engine/prehistoric/core/util/Includes.hpp"
#include <glew.h>
#include "VkPipeline.h"
#include "engine/platform/vulkan/framework/swapchain/VKSwapchain.h"
#include "engine/platform/vulkan/rendering/shaders/VkShader.h"
#include "engine/platform/vulkan/framework/context/VKContext.h"

VKPipeline::VKPipeline(Shader* shader) : Pipeline(shader)
{
	auto stages = static_cast<VKShader*>(shader)->GetShaderStages();
	shaderStageCreateInfos = &stages;

	this->shader = shader;
}

VKPipeline::~VKPipeline()
{
}

void VKPipeline::CreatePipeline(Window* window, MeshVBO* vbo)
{
	VKSwapchain* swapchain = static_cast<VKSwapchain*>(window->GetSwapchain());
	VKContext* context = static_cast<VKContext*>(window->GetContext());

	this->surface = &context->GetSurface();
	this->physicalDevice = (VKPhysicalDevice*) context->GetPhysicalDevice();
	this->device = (VKDevice*) context->GetDevice();

	auto stages = static_cast<VKShader*>(shader)->GetShaderStages();
	this->shaderStageCreateInfos = &stages;

	this->swapchain = swapchain;
	auto format = swapchain->GetSwapchainImageFormat();
	this->imageFormat = &format;
	this->swapchainExtent = &swapchain->GetSwapchainExtent();
	this->swapchainImageViews = swapchain->GetSwapchainImageViews();

	this->commandPool = &swapchain->GetCommandPool();

	this->window = window;

	this->vbo = (VKMeshVBO*) vbo;

	((VKShader*) shader)->CreateShader(swapchain);

	renderpass = new VKRenderpass(device->GetDevice(), *imageFormat);
	graphicsPipeline = new VKGraphicsPipeline(*device, (VKShader*)shader, *renderpass, viewportStart, viewportSize, scissorStart, scissorSize, backfaceCulling, *this->vbo);

	swapchainFramebuffers.resize(swapchainImageViews.size());
	for (size_t i = 0; i < swapchainImageViews.size(); i++)
	{
		swapchainFramebuffers[i] = new VKFramebuffer(*renderpass, *swapchainExtent, swapchainImageViews[i], device->GetDevice());
	}
}

void VKPipeline::RecreatePipeline()
{
	//We delete the old objects
	for (auto framebuffer : swapchainFramebuffers)
	{
		delete framebuffer;
	}

	delete graphicsPipeline;
	delete renderpass;

	this->viewportStart = { 0, 0 };
	this->viewportSize = { (float) FrameworkConfig::windowWidth, (float) FrameworkConfig::windowHeight };
	this->scissorStart = { 0, 0 };
	this->scissorSize = { FrameworkConfig::windowWidth, FrameworkConfig::windowHeight };

	//We need some new values
	VKSwapchain* swapchain = static_cast<VKSwapchain*>(window->GetSwapchain());
	VKContext* context = static_cast<VKContext*>(window->GetContext());

	this->surface = &context->GetSurface();
	this->physicalDevice = (VKPhysicalDevice*) context->GetPhysicalDevice();
	this->device = (VKDevice*) context->GetDevice();

	auto stages = static_cast<VKShader*>(shader)->GetShaderStages();
	this->shaderStageCreateInfos = &stages;

	this->swapchain = swapchain;
	auto format = swapchain->GetSwapchainImageFormat();
	this->imageFormat = &format;
	this->swapchainExtent = &swapchain->GetSwapchainExtent();
	this->swapchainImageViews = swapchain->GetSwapchainImageViews();

	this->commandPool = &swapchain->GetCommandPool();
	
	//Create new objects
	renderpass = new VKRenderpass(device->GetDevice(), *imageFormat);
	graphicsPipeline = new VKGraphicsPipeline(*device, (VKShader*) shader, *renderpass, viewportStart, viewportSize, scissorStart, scissorSize, backfaceCulling, *this->vbo);

	swapchainFramebuffers.resize(swapchainImageViews.size());
	for (size_t i = 0; i < swapchainImageViews.size(); i++)
	{
		swapchainFramebuffers[i] = new VKFramebuffer(*renderpass, *swapchainExtent, swapchainImageViews[i], device->GetDevice());
	}
}

void VKPipeline::BindPipeline()
{
	uint32_t index = window->GetSwapchain()->GetAquiredImageIndex();
	VKCommandBuffer* buff = commandPool->GetCommandBuffer(index);

	this->drawCommandBuffer = &buff->GetCommandBuffer();

	commandPool->BindCommandBuffer(index);

	shader->Bind(buff);

	renderpass->BeginRenderpass(*buff, *swapchainExtent, *(swapchainFramebuffers[index]));
	graphicsPipeline->BindGraphicsPipeline(*buff);
}

void VKPipeline::UnbindPipeline() const
{
	uint32_t index = window->GetSwapchain()->GetAquiredImageIndex();
	VKCommandBuffer* buff = commandPool->GetCommandBuffer(index);

	shader->Unbind();

	renderpass->EndRenderpass(*buff);
	commandPool->UnbindCommandBuffer(index);
}

void VKPipeline::DestroyPipeline()
{
	for (auto framebuffer : swapchainFramebuffers)
	{
		delete framebuffer;
	}

	delete graphicsPipeline;
	delete renderpass;
}