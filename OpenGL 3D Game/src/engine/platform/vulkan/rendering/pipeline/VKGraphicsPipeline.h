#ifndef VK_GRAPHICS_PIPELINE_H
#define VK_GRAPHICS_PIPELINE_H

#include "engine/prehistoric/common/rendering/pipeline/GraphicsPipeline.h"
#include "VKPipeline.h"

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/rendering/renderpass/VKRenderpass.h"
#include "engine/platform/vulkan/buffer/VKMeshVertexBuffer.h"
#include "engine/platform/vulkan/rendering/shaders/VkShader.h"

#include "engine/prehistoric/core/math/Math.h"

class VKGraphicsPipeline : public VKPipeline, public GraphicsPipeline
{
public:
	VKGraphicsPipeline(AssetManager* manager, size_t shaderID, size_t vboID);
	virtual ~VKGraphicsPipeline();

	virtual void CreatePipeline(Window* window) override;

	virtual void BindPipeline() const override;
	virtual void RenderPipeline() const override;
	virtual void UnbindPipeline() const override;

	virtual void DestroyPipeline() override;

	virtual void RecreatePipeline();

	VkPipeline getGraphicsPipeline() const { return graphicsPipeline; }
private:
	VKRenderpass* renderpass;

	VkPipeline graphicsPipeline;
};

#endif