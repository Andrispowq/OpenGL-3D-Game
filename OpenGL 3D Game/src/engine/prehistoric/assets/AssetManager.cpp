#include "engine/prehistoric/core/util/Includes.hpp"
#include "AssetManager.h"

AssetManager::AssetManager(Window* window)
{
	this->window = window;

	textures.reserve(TexturesSize);
	vertexBuffers.reserve(VertexBuffersSize);
	shaders.reserve(ShadersSize);
}

AssetManager::~AssetManager()
{
	for (auto& texture : textures)
	{
		delete texture.second;
	}

	for (auto& vertexBuffer : vertexBuffers)
	{
		delete vertexBuffer.second;
	}

	for (auto& shader : shaders)
	{
		delete shader.second;
	}
}

size_t AssetManager::addTexture(Texture* texture)
{
	textures.insert(std::make_pair(texture_ID, texture));
	return texture_ID++;
}

size_t AssetManager::addVertexBuffer(VertexBuffer* vertexBuffer)
{
	vertexBuffers.insert(std::make_pair(vertexBuffer_ID, vertexBuffer));
	return vertexBuffer_ID++;
}

size_t AssetManager::addShader(Shader* shader)
{
	shaders.insert(std::make_pair(shader_ID, shader));
	return shader_ID++;
}
