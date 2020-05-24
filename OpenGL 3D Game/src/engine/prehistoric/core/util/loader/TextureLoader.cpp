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

		//TODO: get rid of this
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

		if (FrameworkConfig::api == OpenGL)
		{
			texture = new GLTexture();

			texture->setWidth(static_cast<uint32_t>(width));
			texture->setHeight(static_cast<uint32_t>(height));

			texture->Generate();
			texture->Bind();

			if (channels == 3)
			{
				if ((width & 3) != 0)
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 2 - (width & 1));
				}

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)data);
			}
			else if (channels == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)data);
			}
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			texture = new VKTexture(*((VKPhysicalDevice*)window->GetContext()->GetPhysicalDevice()), *((VKDevice*)window->GetContext()->GetDevice()), width, height);

			static_cast<VKTexture*>(texture)->UpdateStagingBuffer((size_t)width * height * 4, data, RGBA32FLOAT);
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