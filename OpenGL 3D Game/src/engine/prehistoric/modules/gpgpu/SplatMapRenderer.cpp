#include "engine/prehistoric/core/util/Includes.hpp"
#include "SplatMapRenderer.h"

SplatMapRenderer::SplatMapRenderer(Window* window, uint32_t N)
{
	this->window = window;

	this->N = N;

	//TODO: VK PIPELINE
	if (FrameworkConfig::api == OpenGL)
	{
		pipeline = new GLComputePipeline(new GLSplatMapShader());
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//pipeline = new VKComputePipeline(new VKSplatMapShader());
	}

	pipeline->SetViewportStart({ 0, 0 });
	pipeline->SetViewportSize({ (float)FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight });
	pipeline->SetScissorStart({ 0, 0 });
	pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

	pipeline->CreatePipeline(window);

	//TODO: TEXTURES
	if (FrameworkConfig::api == OpenGL)
	{
		splatmap = GLTexture::Storage2D(N, N, (uint32_t) (log(N) / log(2)), RGBA32FLOAT, Bilinear);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//splatmap = VKTexture::Storage2D(N, N, (uint32_t) (log(N) / log(2)), RGBA32FLOAT, Bilinear);
	}

	if (FrameworkConfig::api == OpenGL)
	{
		GLComputePipeline* glPipe = reinterpret_cast<GLComputePipeline*>(pipeline);
		glPipe->setInvocationSize({ N / 16, N / 16, 1 });
		glPipe->addTextureBinding(0, splatmap, WRITE_ONLY);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		/*VKComputePipeline* vkPipe = reinterpret_cast<VKComputePipeline*>(pipeline);
		vkPipe->setInvocationSize({ float(N / 16), float(N / 16), 1.0f });
		vkPipe->addTextureBinding(0, splatmap, WRITE_ONLY);*/
	}
}

SplatMapRenderer::~SplatMapRenderer()
{
	pipeline->DestroyPipeline();

	delete pipeline;
	delete splatmap;
}

void SplatMapRenderer::Render(Texture* normalmap)
{
	pipeline->BindPipeline();

	if (FrameworkConfig::api == OpenGL)
	{
		reinterpret_cast<GLSplatMapShader*>(pipeline->getShader())->UpdateUniforms(normalmap, N);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//reinterpret_cast<VKSplatMapShader*>(pipeline->getShader())->UpdateUnifroms(normalmap, N);
	}

	pipeline->RenderPipeline();
	pipeline->UnbindPipeline();

	splatmap->Bind();
	splatmap->SamplerProperties(Bilinear, Repeat);
}