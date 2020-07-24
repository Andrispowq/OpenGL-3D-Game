#include "engine/prehistoric/core/util/Includes.hpp"
#include "Atmosphere.h"

#include "engine/prehistoric/core/Engine.h"

Atmosphere::Atmosphere(Window* window, AssetManager* manager)
	: window(window)
{
	SetScale(Vector3f(EngineConfig::rendererFarPlane / 2.f));

	size_t vboID = manager->addVertexBuffer(OBJLoader::LoadModel("res/models/dome/", "sphericalDome.obj", "", window, manager));

	size_t shaderID = -1;
	Pipeline* pipeline = nullptr;

	if (FrameworkConfig::api == OpenGL)
	{
		if (AtmosphereConfig::scatteringEnabled)
			shaderID = manager->addShader(new GLAtmosphereScatteringShader());
		else
			shaderID = manager->addShader(new GLAtmosphereShader());

		pipeline = new GLGraphicsPipeline(manager, shaderID, vboID);
		reinterpret_cast<GLGraphicsPipeline*>(pipeline)->SetBackfaceCulling(false);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		/*if (AtmosphereConfig::scatteringEnabled)
			shaderID = manager->addShader(new VKAtmosphereScatteringShader());
		else
			shaderID = manager->addShader(new VKAtmosphereShader());*/

		pipeline = new VKGraphicsPipeline(manager, shaderID, vboID);
		reinterpret_cast<VKGraphicsPipeline*>(pipeline)->SetBackfaceCulling(false);
	}

	pipeline->CreatePipeline(window);

	AddComponent(RENDERER_COMPONENT, new Renderer(pipeline, nullptr, window));

	sunPosition = AtmosphereConfig::sunPosition;
}

void Atmosphere::PreUpdate(Engine* engine)
{
	//sun logic can go in here
	if(engine->GetRenderingEngine()->getSun() != nullptr)
		sunPosition = engine->GetRenderingEngine()->getSun()->GetParent()->getWorldTransform()->GetPosition();
}
