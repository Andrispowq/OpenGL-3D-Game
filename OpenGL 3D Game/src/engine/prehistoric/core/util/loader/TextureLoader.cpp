#include "engine/prehistoric/core/util/Includes.hpp"
#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace TextureLoader
{
	Texture* TextureLoader::LoadTexture(const std::string& path, Window* window, SamplerFilter filter, TextureWrapMode wrapMode)
	{
		if(FrameworkConfig::api == OpenGL)
			stbi_set_flip_vertically_on_load(0);

		int width, height, channels;

		unsigned char* data;
		if (FrameworkConfig::api == OpenGL)
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		else
			data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (data == nullptr)
		{
			PR_LOG_ERROR("Failed to load texture %s!\n", path.c_str());
		}

		Texture* texture = nullptr;

		ImageFormat format;
		if (channels == 3 && FrameworkConfig::api != Vulkan) //Vulkan can't decide if it's supports 24 bit textures yet, so don't do this
			format = R8G8B8_LINEAR;
		else
			format = R8G8B8A8_LINEAR;

		//We generally want to generate the texture, then upload the data, but in Vulkan, uploading the data actually loads everything into a staging buffer which
		//is then copied into the actual texture during generation
		if (FrameworkConfig::api == OpenGL)
		{
			texture = new GLTexture();

			texture->setWidth(static_cast<uint32_t>(width));
			texture->setHeight(static_cast<uint32_t>(height));

			texture->Generate();
			texture->Bind();

			texture->UploadTextureData((size_t)width * height * channels, channels, data, format);
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			texture = new VKTexture(*((VKPhysicalDevice*)window->GetContext()->GetPhysicalDevice()), *((VKDevice*)window->GetContext()->GetDevice()), width, height);

			texture->UploadTextureData((size_t)width * height * 4, channels, data, format);

			texture->Generate();
			texture->Bind();
		}

		texture->SamplerProperties(filter, wrapMode);
		texture->Unbind();

		stbi_image_free(data);

		return texture;
	}

	ImageData TextureLoader::LoadTextureData(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
		{
			PR_LOG_ERROR("Failed to load texture %s!\n", path.c_str());
		}

		return { width, height, data };
	}
};