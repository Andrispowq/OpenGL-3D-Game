#include "engine/prehistoric/core/util/Includes.hpp"
#include "GraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(AssetManager* manager, size_t vboID)
{
	this->manager = manager;
	this->vboID = vboID;
	this->backfaceCulling = true;
}

GraphicsPipeline::~GraphicsPipeline()
{
	manager->removeReference<VertexBuffer>(vboID);
	vboID = -1;
}

void GraphicsPipeline::setVertexBufferID(size_t vboID)
{
	this->vboID = vboID;
}