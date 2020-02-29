#include "Renderable.h"
#include "engine/engines/rendering/shaders/Shader.h"

std::vector<VBO*> Renderable::vbos;
std::vector<Shader*> Renderable::shaders;

Renderable::Renderable(VBO* vbo, Shader* shader)
{
	auto vInd = std::find(vbos.begin(), vbos.end(), vbo);

	if (vInd == vbos.end())
	{
		vbos.push_back(vbo);
		this->vboIndex = vbos.size() - 1;
	}
	else
	{
		this->vboIndex = std::distance(vbos.begin(), vInd);
		delete vbo;
	}

	auto sInd = std::find(shaders.begin(), shaders.end(), shader);

	if (sInd == shaders.end())
	{
		shaders.push_back(shader);
		this->shaderIndex = shaders.size() - 1;
	}
	else
	{
		this->shaderIndex = std::distance(shaders.begin(), sInd);
		delete shader;
	}
}

Renderable::Renderable()
{
	vboIndex = -1;
	shaderIndex = -1;
}

Renderable::~Renderable()
{
}

void Renderable::CleanUp()
{
	for (VBO* vbo : vbos)
	{
		delete vbo;
	}

	for (Shader* shader : shaders)
	{
		delete shader;
	}
}