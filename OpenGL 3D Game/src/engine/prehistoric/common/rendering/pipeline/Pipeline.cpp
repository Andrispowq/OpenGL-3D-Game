#include "engine/prehistoric/core/util/Includes.hpp"
#include "Pipeline.h"

#include "engine/config/FrameworkConfig.h"
#include "engine/prehistoric/resources/AssetManager.h"

Pipeline::Pipeline(AssetManager* manager, size_t shaderID)
	: viewportStart{0, 0}, viewportSize{ (float)FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight },
		scissorStart{0, 0}, scissorSize{ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight }
{
	this->assetManager = manager;
	this->shaderID = shaderID;
}

Pipeline::~Pipeline()
{
	assetManager->removeReference<Shader>(shaderID);
	shaderID = -1;
}

void Pipeline::setShaderID(size_t shaderID)
{
	this->shaderID = shaderID;
}