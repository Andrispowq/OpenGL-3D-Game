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
	delete pipeline;
	delete normalmap;
}

void NormalMapRenderer::Render(const Texture* heightmap)
{
	//TODO: TEMPORARY CODE -> Not portable to Vulkan
	pipeline->BindPipeline();
	//TODO: Update Uniforms

	glBindImageTexture(0, normalmap->getID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(N / 16, N / 16, 1);
	glFinish();

	normalmap->Bind();
	normalmap->SamplerProperties(Trilinear, Repeat);

	pipeline->UnbindPipeline();
}