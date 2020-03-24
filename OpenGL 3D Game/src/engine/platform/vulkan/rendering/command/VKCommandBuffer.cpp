#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKCommandBuffer.h"
#include "VKCommandPool.h"

VKCommandBuffer::VKCommandBuffer(VKCommandPool& commandPool)
{
	this->pool = commandPool;

	
}