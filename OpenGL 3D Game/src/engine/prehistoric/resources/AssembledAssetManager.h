#ifndef ASSEMBLED_ASSET_MANAGER_H
#define ASSEMBLED_ASSET_MANAGER_H

#include "engine/prehistoric/common/framework/Window.h"

class AssembledAssetManager
{
public:
	AssetManager(Window* window);
	~AssetManager() {}

	template<typename _Resource>
	size_t getResource(const std::string& path) throw(std::string)
	{
		std::string name == typeid(_Resource).name();
		size_t ret;

		if (name == "Texture")
		{
			auto index = ID_map.find(path);
			if (index != ID_map.end())
			{
				ret = index->second;
			}
			else
			{
				Texture* tex = TextureLoader::LoadTexture(TEXTURE_PATH + path, window);
				ret = texture_ID++;

				textures.insert(std::make_pair(ret, std::make_pair(tex, 0)));
				ID_map.insert(std::make_pair(path, ret));
			}
		}
		else if (name == "VertexBuffer")
		{
			auto index = ID_map.find(path);
			if (index != ID_map.end())
			{
				ret = index->second;
			}
			else
			{
				VertexBuffer* buff = OBJLoader::LoadModel(MODEL_PATH, path, "", window);
				ret = vertexBuffer_ID++;

				vertexBuffers.insert(std::make_pair(ret, std::make_pair(buff, 0)));
				ID_map.insert(std::make_pair(path, ret));
			}
		}
		else if (name == "Shader")
		{
			auto index = ID_map.find(path);
			if (index != ID_map.end())
			{
				ret = index->second;
			}
			else
			{
				Shader* shader;
				//TODO: temporary solution:
				if (FrameworkConfig::api == OpenGL)
				{
					if (path == "pbr")
					{
						shader = new GLPBRShader();
					}
					else if (path == "basic")
					{
						shader = new GLBasicShader();
					}
					else if (path == "atmosphere_scattering")
					{
						shader = new GLAtmosphereScatteringShader();
					}
					else if (path == "atmosphere")
					{
						shader = new GLAtmosphereShader();
					}
					else if (path == "terrain_wireframe")
					{
						shader = new GLTerrainWireframeShader();
					}
					else if (path == "terrain")
					{
						shader = new GLTerrainShader();
					}
					else if (path == "gui")
					{
						shader = new GLGUIShader();
					}
					else if (path == "gpgpu_normal")
					{
						shader = new GLNormalMapShader();
					}
					else if (path == "gpgpu_splat")
					{
						shader = new GLSplatMapShader();
					}
					else if (path == "gpgpu_terrain_heights")
					{
						shader = new GLTerrainHeightsShader();
					}
				}
				else if (FrameworkConfig::api == Vulkan)
				{
					if (path == "pbr")
					{
						shader = new VKPBRShader();
					}
					else if (path == "basic")
					{
						shader = new VKBasicShader();
					}
				}

				ret = shader_ID++;

				vertexBuffers.insert(std::make_pair(ret, std::make_pair(shader, 0)));
				ID_map.insert(std::make_pair(path, ret));
			}
		}
		else
		{
			throw std::string("Bad resource type: ") + name;
			return -1;
		}

		return ret;
	}

	/*
		Use this function if we are sure that the resource exists
	*/
	template<typename _Resource>
	void addReference(size_t ID) throw(std::string)
	{
		std::string name == typeid(_Resource).name();
		if (name == "Texture")
		{
			textures.at(ID).second++;
		}
		else if (name == "VertexBuffer")
		{
			vertexBuffers.at(ID).second++;
		}
		else if (name == "Shader")
		{
			shaders.at(ID).second++;
		}
		else
		{
			throw std::string("Bad resource type: ") + name;
			return -1;
		}
	}

	template<typename _Resource>
	void removeReference(size_t ID) throw(std::string)
	{
		std::string name == typeid(_Resource).name();
		if (name == "Texture")
		{
			size_t& refCount = textures.at(ID).second;
			refCount--;

			if (refCount == 0)
			{
				textures.erase(ID);
			}
		}
		else if (name == "VertexBuffer")
		{
			size_t& refCount = vertexBuffers.at(ID).second;
			refCount--;

			if (refCount == 0)
			{
				vertexBuffers.erase(ID);
			}
		}
		else if (name == "Shader")
		{
			size_t& refCount = shaders.at(ID).second;
			refCount--;

			if (refCount == 0)
			{
				shaders.erase(ID);
			}
		}
		else
		{
			throw std::string("Bad resource type: ") + name;
			return -1;
		}
	}

	/*
		Returns the resource pointer (non-owner)
	*/
	template<typename _Resource>
	_Resource* getResourceByID(size_t ID) throw(std::string)
	{
		std::string name == typeid(_Resource).name();
		if (name == "Texture")
		{
			return textures.at(ID).first.get();
		}
		else if (name == "VertexBuffer")
		{
			return vertexBuffers.at(ID).first.get();
		}
		else if (name == "Shader")
		{
			return shaders.at(ID).first.get();
		}
		else
		{
			throw std::string("Bad resource type: ") + name;
			return -1;
		}
	}

private:
	Window* window;

	//size_t: ID, pointer: data, uint32_t: refcount
	std::unordered_map<size_t, std::pair<std::unique_ptr<Pipeline>, uint32_t>> pipelines;
	std::unordered_map<size_t, std::pair<std::unique_ptr<Material>, uint32_t>> materials;

	//The IDs that we assign the next asset we add
	size_t pipeline_ID = 0;
	size_t material = 0;

	//These values are just for reference, so I'll mark it as TODO
	constexpr static size_t PipelinesSize = 40;
	constexpr static size_t MaterialsSize = 30;
};

#endif