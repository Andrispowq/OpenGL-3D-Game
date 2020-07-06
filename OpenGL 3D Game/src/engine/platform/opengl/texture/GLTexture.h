#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "engine/prehistoric/common/model/Texture.h"

#include <glew.h>

class GLTexture : public Texture
{
public:
	GLTexture(GLuint id = 0, GLenum type = GL_TEXTURE_2D, uint32_t width = 0, uint32_t height = 0);
	GLTexture(GLuint id, ImageType type = TEXTURE_2D, uint32_t width = 0, uint32_t height = 0);
	GLTexture(GLuint id);

	virtual ~GLTexture() override;

	virtual void Bind(uint32_t slot = 0) const override;
	virtual void Unbind() const override;

	virtual void UploadTextureData(size_t size, uint8_t channels, unsigned char* data, ImageFormat format) override;
	virtual void Generate() override;

	virtual void SamplerProperties(SamplerFilter filter, TextureWrapMode wrapMode) override;

	inline GLuint getID() const { return id; }
	inline void setID(GLuint id) { this->id = id; }

	inline void setFormat(ImageFormat format) { this->format = format; }
	inline void setType(ImageType type) { this->type = getImageType(type); }
public:
	//static methods for easier texture creation
	static Texture* GenTexture(const std::string& file, SamplerFilter filter = Trilinear, TextureWrapMode wrapMode = Repeat);
	static Texture* Storage3D(uint32_t width, uint32_t height, uint32_t level = 0, ImageFormat format = R8G8B8A8_LINEAR, SamplerFilter filter = Trilinear, TextureWrapMode wrapMode = Repeat);
	static Texture* Storage2D(uint32_t width, uint32_t height, uint32_t levels = 1, ImageFormat format = R8G8B8A8_LINEAR, SamplerFilter filter = Trilinear, TextureWrapMode wrapMode = Repeat);
	
	static GLenum getImageType(ImageType imageType);
	static GLenum getInternalFormat(ImageFormat format);
	static GLenum getFormat(ImageFormat format);
private:
	GLuint id;
	GLenum type;
};

#endif