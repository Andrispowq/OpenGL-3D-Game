#include "engine/prehistoric/core/util/Includes.hpp"
#include "Texture.h"

int Texture::currID = 0;

Texture::Texture()
{
	ID = currID++;
}

Texture::~Texture()
{
	ID = 0xFFFFFFFF;
}