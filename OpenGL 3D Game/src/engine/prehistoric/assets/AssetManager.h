#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "engine/prehistoric/common/model/Texture.h"
#include "engine/prehistoric/common/buffer/VertexBuffer.h"
#include "engine/prehistoric/common/rendering/shaders/Shader.h"

#include "engine/prehistoric/common/framework/Window.h"

#include <unordered_map>

/*
	This system works like this: every asset is stored here, and when we need one, we just pass in the the size_t ID of the asset, and we get a pointer back
*/
class AssetManager
{
public:
	AssetManager(Window* window);
	virtual ~AssetManager();

	size_t addTexture(Texture* texture);
	size_t addVertexBuffer(VertexBuffer* vertexBuffer);
	size_t addShader(Shader* shader);

	void removeTexture(size_t id) { delete textures.at(id); textures.erase(id); }
	void removeVertexBuffer(size_t id) { delete vertexBuffers.at(id); vertexBuffers.erase(id); }
	void removeShader(size_t id) { delete shaders.at(id); shaders.erase(id); }

	Texture* getTexture(size_t id) const { return textures.at(id); }
	VertexBuffer* getVertexBuffer(size_t id) const { return vertexBuffers.at(id); }
	Shader* getShader(size_t id) const { return shaders.at(id); }

private:
	Window* window;

	//The reason we keep track of textures, shaders and vbos, not only materias and pipelines is because we can assemble new ones from existing ones without the need of reallocation
	std::unordered_map<size_t, Texture*> textures;
	std::unordered_map<size_t, VertexBuffer*> vertexBuffers;
	std::unordered_map<size_t, Shader*> shaders;

	//The IDs that we assign the next asset we add
	size_t texture_ID = 0;	
	size_t vertexBuffer_ID = 0;
	size_t shader_ID = 0;

	//These values are just for reference, so I'll mark it as TODO
	constexpr static size_t TexturesSize = 30;
	constexpr static size_t VertexBuffersSize = 10;
	constexpr static size_t ShadersSize = 10;
};

#endif