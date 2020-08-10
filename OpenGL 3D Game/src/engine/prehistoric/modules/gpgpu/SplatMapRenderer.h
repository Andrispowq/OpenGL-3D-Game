#ifndef SPLAT_MAP_RENDERER_H
#define SPLAT_MAP_RENDERER_H

#include "engine/platform/opengl/texture/GLTexture.h"
#include "engine/platform/vulkan/texture/VKTexture.h"

#include "engine/platform/opengl/rendering/pipeline/GLComputePipeline.h"
//#include "engine/platform/vulkan/rendering/pipeline/VKComputePipeline.h"

#include "engine/platform/opengl/rendering/shaders/gpgpu/GLSplatMapShader.h"
//#include "engine/platform/vulkan/rendering/shaders/gpgpu/VKSplatMapShader.h"

class SplatMapRendererComponent
{
public:
	SplatMapRendererComponent(Window* window, AssetManager* manager, uint32_t N);
	virtual ~SplatMapRendererComponent();

	void Render(Texture* normalmap);

	Texture* getSplatmap() { return splatmap; }
private:
	Window* window;

	Pipeline* pipeline;
	Texture* splatmap;

	uint32_t N;
};

#endif