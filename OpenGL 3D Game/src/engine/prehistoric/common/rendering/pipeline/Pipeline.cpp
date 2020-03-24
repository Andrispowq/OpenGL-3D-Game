#include "engine/prehistoric/core/util/Includes.hpp"
#include "Pipeline.h"

Pipeline::~Pipeline()
{
	delete shader;
}