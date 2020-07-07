#include "engine/prehistoric/core/util/Includes.hpp"
#include "GUIElement.h"

#include "engine/prehistoric/core/util/ModelFabricator.h"

#include "engine/platform/opengl/rendering/shaders/gui/GLGUIShader.h"
#include "engine/platform/vulkan/rendering/shaders/basic/VKBasicShader.h"

#include "engine/prehistoric/core/Engine.h"

VBO* GUIElement::guiVbo = nullptr;
Pipeline* GUIElement::pipeline = nullptr;

GUIElement::GUIElement(Window* window, Texture* texture, void* data, size_t dataSize, bool visible)
	: type(GUIType::Element)
{
	this->texture = texture;
	this->data = data;
	this->dataSize = dataSize;
	this->visible = visible;

	this->window = window;

	if (guiVbo == nullptr)
	{
		guiVbo = ModelFabricator::CreateQuad(window);
		guiVbo->SetFrontFace(FrontFace::CLOCKWISE);
	}

	if (pipeline == nullptr)
	{
		Shader* shader;

		if (FrameworkConfig::api == OpenGL)
		{
			shader = new GLGUIShader();
			pipeline = new GLGraphicsPipeline(shader, guiVbo);
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			shader = new VKBasicShader(window);
			pipeline = new VKGraphicsPipeline(shader, guiVbo);
		}

		pipeline->CreatePipeline(window);
	}

	AddComponent(RENDERER_COMPONENT, new Renderer(pipeline, nullptr, window));
}

void GUIElement::PreUpdate(Engine* engine)
{
}

void GUIElement::PreRender(RenderingEngine* renderingEngine)
{
	if (!visible)
		return;

	GameObject::PreRender(renderingEngine);
}

GUIElement::~GUIElement()
{
	//this data is most likely getting a pointer to a stack variable, so it is not ideal to delete it
	//delete data;
}

void GUIElement::CleanUp()
{
	delete guiVbo;
	delete pipeline;
}