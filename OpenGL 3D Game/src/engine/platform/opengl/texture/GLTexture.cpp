#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLTexture.h"

GLTexture::GLTexture(GLuint id, GLenum type, uint32_t width, uint32_t height)
	: id(id), type(type)
{
	this->width = width;
	this->height = height;

	SamplerProperties(Nearest, Repeat);
}

GLTexture::GLTexture(GLuint id, ImageType type, uint32_t width, uint32_t height)
	: id(id), type(getType(type))
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

GLenum GLTexture::getType(ImageType type) const
{
	if (type == TEXTURE_2D)
		return GL_TEXTURE_2D;
	if (type == TEXTURE_ARRAY_2D)
		return GL_TEXTURE_2D_ARRAY;
	else if (type == TEXTURE_CUBE_MAP)
		return GL_TEXTURE_CUBE_MAP;

	PR_LOG_ERROR("Unsupported texture type: " + type);

	return -1;
}