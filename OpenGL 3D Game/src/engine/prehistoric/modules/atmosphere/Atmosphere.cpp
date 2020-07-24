#include "engine/prehistoric/core/util/Includes.hpp"
#include "Atmosphere.h"

#include "engine/prehistoric/core/Engine.h"

Atmosphere::Atmosphere(Window* window)
	: window(window)
{
	SetScale(Vector3f(EngineConfig::rendererFarPlane / 2.f));

	MeshVertexBuffer* vbo = OBJLoader::LoadModel("res/models/dome/", "sphericalDome.obj", "", window);

	Shader* shader = nullptr;
	Pipeline* pipeline = nullptr;

	if (FrameworkConfig::api == OpenGL)
	{
		if (AtmosphereConfig::scatteringEnabled)
			shader = new GLAtmosphereScatteringShader();
		else
			shader = new GLAtmosphereShader();

		pipeline = new GLGraphicsPipeline(shader, vbo);
		reinterpret_cast<GLGraphicsPipeline*>(pipeline)->SetBackfaceCulling(false);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		/*if (AtmosphereConfig::scatteringEnabled)
			shader = new VKAtmosphereScatteringShader();
		else
			shader = new VKAtmosphereShader();*/

		pipeline = new VKGraphicsPipeline(shader, vbo);
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
