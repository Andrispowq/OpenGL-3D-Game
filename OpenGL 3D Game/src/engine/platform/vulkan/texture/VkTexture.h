#ifndef VK_TEXTURE_H
#define VK_TEXTURE_H

#include "engine/prehistoric/common/model/Texture.h"

#include <vulkan/vulkan.h>

#include "engine/platform/vulkan/framework/device/VKDevice.h"

class VKTexture : public Texture
{
public:
	VKTexture(VKPhysicalDevice& physicalDevice, VKDevice& device, uint32_t width, uint32_t height);
	virtual ~VKTexture();

	void UpdateStagingBuffer(size_t size, unsigned char* pixels);

	virtual void Bind(uint32_t slot = 0) const override;
	virtual void Unbind() const override;

	virtual void Generate() override;

	virtual void SamplerProperties(SamplerFilter filter, TextureWrapMode wrapMode) override;

	VkImageView& GetTextureImageView() { return textureImageView; }
	VkSampler& GetTextureSampler() { return textureSampler; }
private:
	VKDevice* device;
	VKPhysicalDevice* physicalDevice;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	VkImageView textureImageView;
	VkSampler textureSampler;
};

#endif