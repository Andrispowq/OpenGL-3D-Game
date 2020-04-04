#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

#include <fstream>

#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/common/buffer/MeshVBO.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/prehistoric/common/rendering/shaders/Shader.h"
#include "engine/prehistoric/component/renderer/Renderer.h"
#include "engine/prehistoric/core/util/loader/obj/OBJLoader.h"

#include "engine/platform/opengl/rendering/shaders/basic/GLBasicShader.h"
//#include "engine/platform/vulkan/rendering/shaders/basic/VKBasicShader.h"

#include "engine/platform/opengl/rendering/shaders/pbr/GLPBRShader.h"
//#include "engine/platform/vulkan/rendering/shaders/pbr/VKPBRShader.h"

#include "engine/platform/opengl/rendering/pipeline/GLPipeline.h"
#include "engine/platform/vulkan/rendering/pipeline/VKPipeline.h"

#include "engine/prehistoric/core/util/Includes.hpp"

class WorldLoader
{
public:
	WorldLoader() {}
	virtual ~WorldLoader() {}

	void LoadWorld(const std::string& worldFile, GameObject* root, Window* window);
private:
	std::string directoryModels;
	std::string directoryTextures;

	//Rendering stuff
	std::unordered_map<std::string, MeshVBO*> models;
	std::unordered_map<std::string, Texture*> textures;
	std::unordered_map<std::string, Material*> materials;
	std::unordered_map<std::string, Shader*> shaders;
};

#endif