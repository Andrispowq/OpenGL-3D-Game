#include "engine/prehistoric/core/util/Includes.hpp"
#include "Atmosphere.h"

#include "engine/prehistoric/core/Engine.h"
#include "engine/prehistoric/resources/AssembledAssetManager.h"

Atmosphere::Atmosphere(Window* window, AssembledAssetManager* manager)
	: window(window)
{
	SetScale(Vector3f(EngineConfig::rendererFarPlane / 2.f));

	size_t vboID = manager->getAssetManager()->getResource<VertexBuffer>("dome/sphericalDome.obj");

	size_t shaderID = -1;
	Pipeline* pipeline = nullptr;

	if (FrameworkConfig::api == OpenGL)
	{
		if (AtmosphereConfig::scatteringEnabled)
			shaderID = manager->getAssetManager()->getResource<Shader>("atmosphere_scattering");
		else
			shaderID = manager->getAssetManager()->getResource<Shader>("atmosphere");

		pipeline = new GLGraphicsPipeline(window, manager->getAssetManager(), shaderID, vboID);
		reinterpret_cast<GLGraphicsPipeline*>(pipeline)->SetBackfaceCulling(false);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		/*if (AtmosphereConfig::scatteringEnabled)
			shaderID = manager->getAssetManager()->getResource<Shader>("atmosphere_scattering");
		else
			shaderID = manager->getAssetManager()->getResource<Shader>("atmosphere");*/

		pipeline = new VKGraphicsPipeline(window, manager->getAssetManager(), shaderID, vboID);
		reinterpret_cast<VKGraphicsPipeline*>(pipeline)->SetBackfaceCulling(false);
	}

	AddComponent(RENDERER_COMPONENT, new RendererComponent(pipeline, nullptr, window, manager));

	sunPosition = AtmosphereConfig::sunPosition;
}

void Atmosphere::PreUpdate(Engine* engine)
{
	//sun logic can go in here
	if(engine->getRenderingEngine()->getRenderer()->getSun() != nullptr)
		sunPosition = engine->getRenderingEngine()->getRenderer()->getSun()->getParent()->getWorldTransform().getPosition();
}
