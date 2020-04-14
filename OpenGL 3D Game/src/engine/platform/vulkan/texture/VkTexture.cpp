#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkTexture.h"

VKTexture::VKTexture(VKPhysicalDevice& physicalDevice, VKDevice& device, uint32_t width, uint32_t height)
{
	this->physicalDevice = &physicalDevice;
	this->device = &device;

	this->width = width;
	this->height = height;
}

VKTexture::~VKTexture()
{
	vkDestroySampler(device->GetDevice(), textureSampler, nullptr);
	vkDestroyImageView(device->GetDevice(), textureImageView, nullptr);

	vkDestroyImage(device->GetDevice(), textureImage, nullptr);
	vkFreeMemory(device->GetDevice(), textureImageMemory, nullptr);
}

void VKTexture::UpdateStagingBuffer(size_t size, unsigned char* pixels)
{
	VKUtil::CreateBuffer(physicalDevice->GetPhysicalDevice(), device->GetDevice(), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device->GetDevice(), stagingBufferMemory, 0, size, 0, &data);
	memcpy(data, pixels, size);
	vkUnmapMemory(device->GetDevice(), stagingBufferMemory);
}

void VKTexture::Bind(uint32_t slot) const
{

}

void VKTexture::Unbind() const
{

}

void VKTexture::Generate()
{
	//We can create the image new based on the staging buffer's data
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0; // Optional

	if (vkCreateImage(device->GetDevice(), &imageInfo, nullptr, &textureImage) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to create image!\n");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device->GetDevice(), textureImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VKUtil::FindMemoryType(memRequirements.memoryTypeBits, physicalDevice->GetPhysicalDevice(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &textureImageMemory) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to allocate image memory!\n");
	}

	vkBindImageMemory(device->GetDevice(), textureImage, textureImageMemory, 0);

	VKUtil::TransitionImageLayout(*device, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	VKUtil::CopyBufferToImage(*device, stagingBuffer, textureImage, width, height);
	VKUtil::TransitionImageLayout(*device, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	//We're no longer in need of the staging buffers
	vkDestroyBuffer(device->GetDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), stagingBufferMemory, nullptr);

	//We can now create the image view
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = textureImage;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(device->GetDevice(), &viewInfo, nullptr, &textureImageView) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to create texture image view!\n");
	}
	
	//Creating the sampler, which is based on bilinear sampling with 16x anisotropy filtering, which is a mode likely to be used, but at any time it can be
	//recreate with the method SamplerProperties(SamplerFilter, TextureWrapMode)
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device->GetDevice(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to create texture sampler!\n");
	}
}

void VKTexture::SamplerProperties(SamplerFilter filter, TextureWrapMode wrapMode)
{
	//We recreate the sampler
	vkDestroySampler(device->GetDevice(), textureSampler, nullptr);

	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	switch (filter)
	{
	case Nearest:
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1;
		break;
	case Bilinear:
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1;
		break;
	case Trilinear:
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1;
		break;
	case Anisotropic:
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		break;
	default:
		break;
	}

	switch (wrapMode)
	{
	case ClampToEdge:
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		break;
	case ClampToBorder:
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		break;
	case Repeat:
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		break;
	case MirrorRepeat:
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		break;
	default:
		break;
	}

	if (vkCreateSampler(device->GetDevice(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create texture sampler!\n");
	}
}