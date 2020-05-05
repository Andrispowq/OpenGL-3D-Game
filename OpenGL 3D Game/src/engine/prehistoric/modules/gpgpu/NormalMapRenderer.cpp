#include "engine/prehistoric/core/util/Includes.hpp"
#include "NormalMapRenderer.h"

NormalMapRenderer::NormalMapRenderer(Window* window, float strength, uint32_t N)
{
	this->window = window;

	this->strength = strength;
	this->N = N;

	//TODO: SHADERS
	if (FrameworkConfig::api == OpenGL)
	{

	}
	else if (FrameworkConfig::api == Vulkan)
	{

	}

	//TODO: TEXTURES
	if (FrameworkConfig::api == OpenGL)
	{
		normalmap = GLTexture::Storage2D(N, N, RGBA32FLOAT, Trilinear);
	}
	else if (FrameworkConfig::api == Vulkan)
	{

	}
}

NormalMapRenderer::~NormalMapRenderer()
{
	delete shader;
	delete normalmap;
}

void NormalMapRenderer::Render(const Texture* heightmap)
{
	//TODO: TEMPORARY CODE -> Not portable to Vulkan
	shader->Bind(nullptr);
	
	normalmap->Bind();
}