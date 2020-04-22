#include "engine/prehistoric/core/util/Includes.hpp"
#include "Pipeline.h"

std::vector<Shader*> Pipeline::shaders;
std::vector<VBO*> Pipeline::vbos;

Pipeline::Pipeline(Shader* shader, VBO* vbo)
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

	if ((index = FindElement(vbo, vbos)) == 0xFFFFFFFF)
	{
		vbos.push_back(vbo);
		this->vboIndex = vbos.size() - 1;
	}
	else
	{
		this->vboIndex = index;
	}

	this->shader = shader;
	this->vbo = vbo;
}

Pipeline::~Pipeline()
{
	shaderIndex = -1;
	vboIndex = -1;
}

void Pipeline::CleanUp()
{
	for (Shader* shader : shaders)
	{
		delete shader;
	}

	for (VBO* vbo : vbos)
	{
		delete vbo;
	}
}

void Pipeline::SetShader(Shader* shader)
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

void Pipeline::SetVbo(VBO* vbo)
{
	size_t index;
	if ((index = FindElement(vbo, vbos)) == 0xFFFFFFFF)
	{
		vbos.push_back(vbo);
		this->vboIndex = vbos.size() - 1;
	}
	else
	{
		this->vboIndex = index;
	}

	this->vbo = vbo;
}
