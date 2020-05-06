#include "engine/prehistoric/core/util/Includes.hpp"
#include "Pipeline.h"

std::vector<Shader*> Pipeline::shaders;

Pipeline::Pipeline(Shader* shader)
{
	size_t index;
	if ((index = FindElement(shader, shaders)) == 0xFFFFFFFF)
	{
		shaders.push_back(shader);
		this->shaderIndex = shaders.size() - 1;
	}
	else
	{
		this->shaderIndex = index;
	}

	this->shader = shader;
}

Pipeline::~Pipeline()
{
	shaderIndex = -1;
}

void Pipeline::CleanUp()
{
	for (Shader* shader : shaders)
	{
		delete shader;
	}
}

void Pipeline::setShader(Shader* shader)
{
	size_t index;
	if ((index = FindElement(shader, shaders)) == 0xFFFFFFFF)
	{
		shaders.push_back(shader);
		this->shaderIndex = shaders.size() - 1;
	}
	else
	{
		this->shaderIndex = index;
	}

	this->shader = shader;
}