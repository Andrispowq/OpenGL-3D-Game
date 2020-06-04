#include "engine/prehistoric/core/util/Includes.hpp"
#include "GraphicsPipeline.h"

std::vector<VBO*> GraphicsPipeline::vbos;

GraphicsPipeline::GraphicsPipeline(VBO* vbo)
	: wireframe(false)
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
	this->backfaceCulling = true; //TODO: convert to option
	this->frontFace = FrontFace::COUNTER_CLOCKWISE;
}

GraphicsPipeline::~GraphicsPipeline()
{
	vboIndex = -1;
}

void GraphicsPipeline::CleanUp()
{
	for (VBO* vbo : vbos)
	{
		delete vbo;
	}
}

void GraphicsPipeline::setVbo(VBO* vbo)
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