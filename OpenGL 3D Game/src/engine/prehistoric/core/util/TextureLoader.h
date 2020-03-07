#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>

class Texture;

namespace TextureLoader
{
	Texture* LoadTexture(const std::string& path);
};

#endif