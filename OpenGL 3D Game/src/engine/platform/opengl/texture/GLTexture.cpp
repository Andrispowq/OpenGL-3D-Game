#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLTexture.h"
#include "engine/prehistoric/core/util/loader/TextureLoader.h"

GLTexture::GLTexture(GLuint id, GLenum type, uint32_t width, uint32_t height)
	: id(id), type(type)
{
	this->width = width;
	this->height = height;

	SamplerProperties(Nearest, Repeat);
}

GLTexture::GLTexture(GLuint id, ImageType type, uint32_t width, uint32_t height)
	: id(id), type(getImageType(type))
{
	this->width = width;
	this->height = height;

	SamplerProperties(Nearest, Repeat);
}

GLTexture::GLTexture(GLuint id)
	: id(id), type(GL_TEXTURE_2D)
{
	this->width = 0;
	this->height = 0;

	SamplerProperties(Nearest, Repeat);
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &id);
}

void GLTexture::Bind(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(type, id);
}

void GLTexture::Unbind() const
{
	glBindTexture(type, 0);
}

void GLTexture::Generate()
{
	glGenTextures(1, &id);
}

void GLTexture::SamplerProperties(SamplerFilter filter, TextureWrapMode wrapMode)
{
	switch (filter)
	{
	case Nearest:
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case Bilinear:
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case Trilinear:
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(type);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	case Anisotropic:
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(type);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (GL_EXT_texture_filter_anisotropic)
		{
			GLfloat maxAnisotropyLevel;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropyLevel);
			glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropyLevel);
		}
		else
		{
			PR_LOG_ERROR("Anisotropic filtering is not supported!\n");
		}

		break;
	default:
		PR_LOG_ERROR("This filtering type does not exist for textures! Type: %u\n", filter);
		break;
	}

	switch (wrapMode)
	{
	case ClampToEdge:
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (type == GL_TEXTURE_CUBE_MAP)
			glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		break;
	case ClampToBorder:
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		if (type == GL_TEXTURE_CUBE_MAP)
			glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		break;
	case Repeat:
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (type == GL_TEXTURE_CUBE_MAP)
			glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_REPEAT);
		break;
	case MirrorRepeat:
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		if (type == GL_TEXTURE_CUBE_MAP)
			glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
		break;
	default:
		PR_LOG_ERROR("This type of texture wrapping does not exist! Type: %u\n", wrapMode);
		break;
	}
}

Texture* GLTexture::GenTexture(const std::string& file, SamplerFilter filter, TextureWrapMode wrapMode)
{
	Texture* texture = TextureLoader::LoadTexture(file, nullptr);
	texture->Bind();
	texture->SamplerProperties(filter, wrapMode);
	return texture;
}

Texture* GLTexture::Storage3D(uint32_t width, uint32_t height, ImageFormat format, SamplerFilter filter, TextureWrapMode wrapMode)
{
	GLTexture* texture = new GLTexture();
	texture->Generate();
	texture->setType(TEXTURE_CUBE_MAP);
	texture->Bind();
	texture->setWidth(width);
	texture->setHeight(height);

	for (uint32_t i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, getInternalFormat(format), width, height, 0, getFormat(format), GL_FLOAT, nullptr);
	}

	texture->SamplerProperties(filter, wrapMode);
	texture->Unbind();
	return texture;
}

Texture* GLTexture::Storage2D(uint32_t width, uint32_t height, ImageFormat format, SamplerFilter filter, TextureWrapMode wrapMode)
{
	GLTexture* texture = new GLTexture();
	texture->Generate();
	texture->setType(TEXTURE_2D);
	texture->Bind();
	texture->setWidth(width);
	texture->setHeight(height);

	//glTexImage2D(GL_TEXTURE_2D, 0, getInternalFormat(format), width, height, 0, getFormat(format), GL_FLOAT, nullptr);
	glTexStorage2D(GL_TEXTURE_2D, 1, getInternalFormat(format), width, height); //This does not work for some reason, though the Java version uses this method

	texture->SamplerProperties(filter, wrapMode);
	texture->Unbind();
	return texture;
}

GLenum GLTexture::getImageType(ImageType type)
{
	if (type == TEXTURE_2D)
		return GL_TEXTURE_2D;
	else if (type == TEXTURE_ARRAY_2D)
		return GL_TEXTURE_2D_ARRAY;
	else if (type == TEXTURE_CUBE_MAP)
		return GL_TEXTURE_CUBE_MAP;

	PR_LOG_ERROR("Unsupported texture type: %u\n", type);

	return -1;
}

GLenum GLTexture::getInternalFormat(ImageFormat format)
{
	if (format == R8FLOAT)
		return GL_R8;
	if (format == R16FLOAT)
		return GL_R16;
	if (format == R32FLOAT)
		return GL_R32F;
	if (format == RG16FLOAT)
		return GL_RG16;
	if (format == RG32FLOAT)
		return GL_RG32F;
	if (format == RGB24FLOAT)
		return GL_RGB32F;
	if (format == RGBA16FLOAT)
		return GL_RGBA16F;
	if (format == RGBA32FLOAT)
		return GL_RGBA32F;

	PR_LOG_ERROR("Unsupported texture format: %u\n", format);

	return -1;
}

GLenum GLTexture::getFormat(ImageFormat format)
{
	if (format == R8FLOAT)
		return GL_R;
	if (format == R16FLOAT)
		return GL_R;
	if (format == R32FLOAT)
		return GL_R;
	if (format == RG16FLOAT)
		return GL_RG;
	if (format == RG32FLOAT)
		return GL_RG;
	if (format == RGB24FLOAT)
		return GL_RGB;
	if (format == RGBA16FLOAT)
		return GL_RGBA;
	if (format == RGBA32FLOAT)
		return GL_RGBA;

	PR_LOG_ERROR("Unsupported texture format: %u\n", format);

	return -1;
}