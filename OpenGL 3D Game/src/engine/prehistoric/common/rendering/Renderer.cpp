#include "engine/prehistoric/core/util/Includes.hpp"
#include "Renderer.h"

#include "engine/prehistoric/component/renderer/RenderableComponent.h"

#include "engine/prehistoric/component/light/Light.h"

void Renderer::PrepareRendering()
{
	//No code yet
}

void Renderer::EndRendering()
{
	window->Render();

	//Clear the lists for the next iteration
	models_3d.clear();
	models_transparency.clear();
	models_2d.clear();
	lights.clear();
}

void register_model(std::unordered_map<Pipeline*, std::vector<RenderableComponent*>>& map, RenderableComponent* renderable)
{
	Pipeline* pipeline = renderable->getPipeline();

	if (map.find(pipeline) != map.end())
	{
		auto& renderables = map[pipeline];
		renderables.push_back(renderable);
	}
	else
	{
		std::vector<RenderableComponent*> renderers = { renderable };
		map.insert(std::make_pair(pipeline, renderers));
	}
}

void Renderer::AddModel(RenderableComponent* renderable)
{
	switch (renderable->getPriority())
	{
	case RenderPriority::_3D:
		register_model(models_3d, renderable);
		break;
	case RenderPriority::_TRANSPARENCY:
		register_model(models_transparency, renderable);
		break;
	case RenderPriority::_2D:
		register_model(models_2d, renderable);
		break;
	default:
		break;
	}
}

void Renderer::AddLight(Light* light)
{
	if (light->isSun())
		sun = light;

	lights.push_back(light);
}
