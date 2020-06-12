#include "engine/prehistoric/core/util/Includes.hpp"
#include "NormalMapRenderer.h"

NormalMapRenderer::NormalMapRenderer(Window* window, float strength, uint32_t N)
{
	this->window = window;

	this->strength = strength;
	this->N = N;

	//TODO: VK PIPELINE
	if (FrameworkConfig::api == OpenGL)
	{
		pipeline = new GLComputePipeline(new GLNormalMapShader());
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//pipeline = new VKComputePipeline(new VKNormalMapShader());
	}

	pipeline->SetViewportStart({ 0, 0 });
	pipeline->SetViewportSize({ (float) FrameworkConfig::windowWidth, (float) FrameworkConfig::windowHeight });
	pipeline->SetScissorStart({ 0, 0 });
	pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

	pipeline->CreatePipeline(window);

	//TODO: TEXTURES
	if (FrameworkConfig::api == OpenGL)
	{
		normalmap = GLTexture::Storage2D(N, N, (uint32_t) (log(N) / log(2)), R8G8B8A8_LINEAR, Bilinear);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//normalmap = VKTexture::Storage2D(N, N, (uint32_t) (log(N) / log(2)), R8G8B8A8_LINEAR, Bilinear);
	}

	if (FrameworkConfig::api == OpenGL)
	{
		GLComputePipeline* glPipe = reinterpret_cast<GLComputePipeline*>(pipeline);
		glPipe->setInvocationSize({ N / 16, N / 16, 1 });
		glPipe->addTextureBinding(0, normalmap, WRITE_ONLY);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		/*VKComputePipeline* vkPipe = reinterpret_cast<VKComputePipeline*>(pipeline);
		vkPipe->setInvocationSize({ float(N / 16), float(N / 16), 1.0f });
		vkPipe->addTextureBinding(0, normalmap, WRITE_ONLY);*/
	}
}

NormalMapRenderer::~NormalMapRenderer()
{
	pipeline->DestroyPipeline();

	delete pipeline;
	delete normalmap;
}

void NormalMapRenderer::Render(Texture* heightmap)
{
	pipeline->BindPipeline();

	if (FrameworkConfig::api == OpenGL)
	{
		reinterpret_cast<GLNormalMapShader*>(pipeline->getShader())->UpdateUniforms(heightmap, N, strength);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//reinterpret_cast<VKNormalMapShader*>(pipeline->getShader())->UpdateUnifroms(heightmap, N, strength);
	}

	pipeline->RenderPipeline();
	pipeline->UnbindPipeline();

	normalmap->Bind();
	normalmap->SamplerProperties(Bilinear, Repeat);
}