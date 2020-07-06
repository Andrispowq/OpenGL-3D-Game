#ifndef VK_GRAPHICS_PIPELINE_H
#define VK_GRAPHICS_PIPELINE_H

#include "engine/prehistoric/common/rendering/pipeline/GraphicsPipeline.h"
#include "VKPipeline.h"

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/rendering/renderpass/VKRenderpass.h"
#include "engine/platform/vulkan/buffer/VKMeshVBO.h"
#include "engine/platform/vulkan/rendering/shaders/VkShader.h"

#include "engine/prehistoric/core/math/Math.h"

class VKGraphicsPipeline : public VKPipeline, public GraphicsPipeline
{
public:
	VKGraphicsPipeline(Shader* shader, VBO* vbo);
	virtual ~VKGraphicsPipeline();

	virtual void CreatePipeline(Window* window) override;

	virtual void BindPipeline() const override;
	virtual void RenderPipeline() const override;
	virtual void UnbindPipeline() const override;

	virtual void DestroyPipeline() override;

	virtual void RecreatePipeline();

	VkPipeline& GetGraphicsPipeline() { return graphicsPipeline; }
private:
	VkPipelineLayout* pipelineLayout;
	VKRenderpass* renderpass;

	VkPipeline graphicsPipeline;
};

#endif