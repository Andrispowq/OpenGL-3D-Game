#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLRenderer.h"

void GLRenderer::Render(Camera* camera)
{
	for (auto pipeline : models_3d)
	{
		Pipeline* pl = pipeline.first;

		pl->BindPipeline();
		pl->getShader()->UpdateShaderUniforms(camera, lights);
		pl->getShader()->UpdateSharedUniforms(pipeline.second[0]->getParent()); //Safe -> there is at least 1 element in the array

		for (auto renderer : pipeline.second)
		{
			renderer->BatchRender(*this);
		}

		pl->UnbindPipeline();
	}

	//TODO: enable alpha blending
	for (auto pipeline : models_transparency)
	{
		Pipeline* pl = pipeline.first;

		pl->BindPipeline();
		pl->getShader()->UpdateShaderUniforms(camera, lights);
		pl->getShader()->UpdateSharedUniforms(pipeline.second[0]->getParent()); //Safe -> there is at least 1 element in the array

		for (auto renderer : pipeline.second)
		{
			renderer->BatchRender(*this);
		}

		pl->UnbindPipeline();
	}

	//TODO: disable alpha blending and depth testing
	for (auto pipeline : models_2d)
	{
		Pipeline* pl = pipeline.first;

		pl->BindPipeline();

		for (auto renderer : pipeline.second)
		{
			renderer->BatchRender(*this);
		}

		pl->UnbindPipeline();
	}
}
