#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "glew.h"

#include "engine/prehistoric/common/model/Texture.h"

class GLTexture : public Texture
{
public:
	GLTexture(GLuint id = 0, GLenum type = GL_TEXTURE_2D, unsigned int width = 0, unsigned int height = 0);
	GLTexture(GLuint id, ImageType type = TEXTURE_2D, unsigned int width = 0, unsigned int height = 0);
	GLTexture(GLuint id);

	virtual ~GLTexture() override;

	virtual void Bind(unsigned int slot = 0) const override;
	virtual void Unbind() const override;

	virtual void Generate() override;

	virtual void Filter(SamplerFilter filter) const override;
	virtual void WrapMode(TextureWrapMode wrapMode) const override;

	inline GLuint getID() const { return id; }
	inline void setID(GLuint id) { this->id = id; }
private:
	GLenum getType(ImageType imageType) const;
private:
	GLuint id;
	GLenum type;
};

#endif