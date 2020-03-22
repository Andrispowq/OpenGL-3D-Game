#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "engine/prehistoric/core/util/Includes.hpp"

class Texture;

struct ImageData
{
	int width, height;
	unsigned char* data;
};

namespace TextureLoader
{
	Texture* LoadTexture(const std::string& path);
	ImageData LoadTextureData(const std::string& path);
};

#endif