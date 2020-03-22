#ifndef TEXTURE_H
#define TEXTURE_H

enum ImageFormat
{
	RGBA32FLOAT, RGB32FLOAT, RGBA16FLOAT, DEPTH32FLOAT, R16FLOAT, R32FLOAT, R8FLOAT, RG16FLOAT, RG32FLOAT
};

enum ImageType
{
	TEXTURE_2D, TEXTURE_ARRAY_2D, TEXTURE_CUBE_MAP
};

enum SamplerFilter
{
	Nearest, Bilinear, Trilinear, Anisotropic
};

enum TextureWrapMode
{
	ClampToEdge, ClampToBorder, Repeat, MirrorRepeat
};

class Texture
{
public:
	Texture() : width(0), height(0) {}

	virtual ~Texture() = 0;

	virtual void Bind(unsigned int slot = 0) const = 0;
	virtual void Unbind() const = 0;

	virtual void Generate() = 0;

	virtual void Filter(SamplerFilter filter) const = 0;
	virtual void WrapMode(TextureWrapMode wrapMode) const = 0;

	inline unsigned int getWidth() const { return width; }
	inline unsigned int getHeight() const { return height; }

	inline void setWidth(unsigned int width) { this->width = width; }
	inline void setHeight(unsigned int height) { this->height = height; }
protected:
	unsigned int width, height;
};

#endif