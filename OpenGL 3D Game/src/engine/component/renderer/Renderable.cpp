#include "Renderable.h"
#include "engine/engines/rendering/shaders/Shader.h"

std::vector<VBO*> Renderable::vbos;

Renderable::Renderable(VBO* vbo, Shader* shader)
{
	auto index = std::find(vbos.begin(), vbos.end(), vbo);

	if (index == vbos.end())
	{
		vbos.push_back(vbo);
		this->vboIndex = vbos.size() - 1;
	}
	else
	{
		this->vboIndex = std::distance(vbos.begin(), index);
	}

	this->shader = shader;
}

Renderable::Renderable()
{
	vboIndex = -1;
	shader = nullptr;
}

Renderable::~Renderable()
{
	//if(shader != nullptr)
	//	delete shader;
}

void Renderable::CleanUp()
{
	for (VBO* vbo : vbos)
	{
		delete vbo;
	}
}