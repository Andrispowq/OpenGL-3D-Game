#ifndef VK_GRAPHICS_PIPELINE_H
#define VK_GRAPHICS_PIPELINE_H

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"
#include "engine/platform/vulkan/rendering/renderpass/VKRenderpass.h"
#include "engine/platform/vulkan/buffer/VKMeshVBO.h"
#include "engine/platform/vulkan/rendering/shaders/VkShader.h"

#include "engine/prehistoric/core/math/Math.h"

class VKGraphicsPipeline
{
public:
	VKGraphicsPipeline(VKDevice& device, VKShader* shader, VKRenderpass& renderpass,
		const Vector2f& viewportStart, const Vector2f& viewportSize, const Vector2u& scissorStart, const Vector2u& scissorSize, const bool& backfaceCulling, 
		VKMeshVBO& vbo, VkSampleCountFlagBits samples);
	virtual ~VKGraphicsPipeline();

	void BindGraphicsPipeline(VKCommandBuffer& commandBuffer) const;

	VkPipeline& GetGraphicsPipeline() { return graphicsPipeline; }
private:
	VKDevice* device;
	VkPipelineLayout* pipelineLayout;

	VkPipeline graphicsPipeline;

	Vector2f viewportStart;
	Vector2f viewportSize;
	Vector2u scissorStart;
	Vector2u scissorSize;

	bool backfaceCulling;
};

#endif