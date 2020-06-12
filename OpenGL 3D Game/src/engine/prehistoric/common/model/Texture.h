#ifndef TEXTURE_H
#define TEXTURE_H

//from vulkan.h
enum ImageFormat
{
	R8G8B8A8_SRGB, R8G8B8_SRGB, D32_SFLOAT, R16_SFLOAT, R32_SFLOAT, R8_SRGB, R8G8_SRGB, R16G16_SFLOAT,
	R8G8B8A8_LINEAR, R8G8B8_LINEAR, D32_LINEAR, R16_LINEAR, R32_LINEAR, R8_LINEAR, R8G8_LINEAR, R16G16_LINEAR
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

	virtual void UploadTextureData(size_t size, uint8_t channels, unsigned char* data, ImageFormat format) = 0;
	virtual void Generate() = 0;

	virtual void SamplerProperties(SamplerFilter filter, TextureWrapMode wrapMode) = 0;

	inline uint32_t getWidth() const { return width; }
	inline uint32_t getHeight() const { return height; }
	inline uint32_t getID() const { return ID; }

	inline void setWidth(uint32_t width) { this->width = width; }
	inline void setHeight(uint32_t height) { this->height = height; }
	inline void setID(uint32_t id) { this->ID = id; }
protected:
	uint32_t width, height;
	uint32_t ID;

	static int currID;
};

#endif