#include "engine/prehistoric/core/util/Includes.hpp"
#include "TextureLoader.h"
#include "engine/platform/opengl/texture/GLTexture.h"
#include "engine/prehistoric/core/util/Util.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace TextureLoader
{
	Texture* TextureLoader::LoadTexture(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(0);

		int width, height, channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
		{
			PR_LOG_ERROR("Failed to load texture %s!\n", path.c_str());
		}

		Texture* texture = new GLTexture();

		texture->setWidth(static_cast<unsigned int>(width));
		texture->setHeight(static_cast<unsigned int>(height));

		texture->Generate();
		texture->Bind();

		if (channels == 3)
		{
			if ((width & 3) != 0)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 2 - (width & 1));
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*) data);
		}
		else if (channels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) data);
		}

		texture->Filter(Anisotropic);
		texture->WrapMode(Repeat);
		
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