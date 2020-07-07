#include "engine/prehistoric/core/util/Includes.hpp"
#include "Light.h"

#include "engine/prehistoric/core/Engine.h"

Light::Light(const Vector3f& colour, const Vector3f& intensity, bool isSun, UpdateFunction function)
	: Component(), colour(colour), intensity(intensity), isSun(isSun)
{
	this->updateFunction = function;
}

void Light::PreUpdate(Engine* engine)
{
	if(updateFunction != nullptr) //non-suns can have move scripts
		updateFunction(parent, engine->getFrameTime());
}

void Light::PreRender(RenderingEngine* renderingEngine)
{
	renderingEngine->AddLight(this);
}