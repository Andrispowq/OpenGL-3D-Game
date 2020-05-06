#ifndef NORMAL_MAP_RENDERER_H
#define NORMAL_MAP_RENDERER_H

#include "engine/platform/opengl/texture/GLTexture.h"
#include "engine/platform/vulkan/texture/VKTexture.h"

#include "engine/platform/opengl/rendering/pipeline/GLPipeline.h"
#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"

class NormalMapRenderer
{
public:
	NormalMapRenderer(Window* window, float strength, uint32_t N);
	virtual ~NormalMapRenderer();

	void Render(const Texture* heightmap);
private:
	Window* window;
	
	Pipeline* pipeline;
	Texture* normalmap;

	float strength;
	uint32_t N;
};

#endif