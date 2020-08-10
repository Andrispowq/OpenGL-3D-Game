#include "engine/prehistoric/core/util/Includes.hpp"
#include "SplatMapRendererComponent.h"

SplatMapRendererComponent::SplatMapRendererComponent(Window* window, AssetManager* manager, uint32_t N)
{
	this->window = window;

	this->N = N;

	//TODO: Create the Vulkan equivalent of the GLComputePipeline
	if (FrameworkConfig::api == OpenGL)
	{
		pipeline = new GLComputePipeline(manager, manager->addShader(new GLSplatMapShader()));
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//pipeline = new VKComputePipeline(new VKSplatMapShader());
	}

	pipeline->CreatePipeline(window);

	if (FrameworkConfig::api == OpenGL)
	{
		splatmap = GLTexture::Storage2D(N, N, (uint32_t) (log(N) / log(2)), R8G8B8A8_LINEAR, Bilinear);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//splatmap = VKTexture::Storage2D(N, N, (uint32_t) (log(N) / log(2)), R8G8B8A8_LINEAR, Bilinear);
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

SplatMapRendererComponent::~SplatMapRendererComponent()
{
	pipeline->DestroyPipeline();

	delete pipeline;
	delete splatmap;
}

void SplatMapRendererComponent::Render(Texture* normalmap)
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