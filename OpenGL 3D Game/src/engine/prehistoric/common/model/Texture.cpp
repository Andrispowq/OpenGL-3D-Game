#include "engine/prehistoric/core/util/Includes.hpp"
#include "Texture.h"

int Texture::currID = 0;

Texture::Texture()
{
	id = currID++;
}

Texture::~Texture()
{
	id = 0xFFFFFFFF;
}