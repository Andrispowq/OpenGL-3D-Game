#include "engine/prehistoric/core/util/Includes.hpp"
#include "Light.h"

void Light::PreRender(RenderingEngine* renderingEngine)
{
	renderingEngine->AddLight(this);
}