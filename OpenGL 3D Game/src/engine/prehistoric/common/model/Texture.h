#ifndef TEXTURE_H
#define TEXTURE_H

enum ImageFormat
{
	RGBA32FLOAT, RGB24FLOAT, RGBA16FLOAT, DEPTH32FLOAT, R16FLOAT, R32FLOAT, R8FLOAT, RG16FLOAT, RG32FLOAT
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
	Texture();

	virtual ~Texture() = 0;

	virtual void Bind(uint32_t slot = 0) const = 0;
	virtual void Unbind() const = 0;

	virtual void Generate() = 0;

	virtual void SamplerProperties(SamplerFilter filter, TextureWrapMode wrapMode) = 0;

	inline uint32_t getWidth() const { return width; }
	inline uint32_t getHeight() const { return height; }
	inline uint32_t getID() const { return id; }

	inline void setWidth(uint32_t width) { this->width = width; }
	inline void setHeight(uint32_t height) { this->height = height; }
	inline void setID(uint32_t id) { this->id = id; }
protected:
	uint32_t width, height;
	uint32_t id;

	static int currID;
};

#endif