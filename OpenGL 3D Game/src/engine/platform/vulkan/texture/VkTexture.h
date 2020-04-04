#ifndef VK_TEXTURE_H
#define VK_TEXTURE_H

#include <vulkan/vulkan.h>

#include "engine/prehistoric/common/model/Texture.h"

class VKTexture : public Texture
{
public:
	VKTexture();
	virtual ~VKTexture();

	virtual void Bind(uint32_t slot = 0) const override;
	virtual void Unbind() const override;

	virtual void Generate() override;

	virtual void Filter(SamplerFilter filter) const override;
	virtual void WrapMode(TextureWrapMode wrapMode) const override;
private:
	VkImage image;
	VkImageView imageView;
};

#endif