#include "engine/prehistoric/core/util/Includes.hpp"
#include "GUIElement.h"

#include "engine/prehistoric/core/util/ModelFabricator.h"

#include "engine/platform/opengl/rendering/shaders/gui/GLGUIShader.h"
#include "engine/platform/vulkan/rendering/shaders/basic/VKBasicShader.h"

VBO* GUIElement::guiVbo = nullptr;
Pipeline* GUIElement::pipeline = nullptr;

GUIElement::GUIElement(Window* window, Texture* texture, void* data, size_t dataSize, bool visible)
{
	this->texture = texture;
	this->data = data;
	this->dataSize = dataSize;
	this->visible = visible;

	this->window = window;

	if (guiVbo == nullptr)
	{
		guiVbo = ModelFabricator::CreateQuad(window);
	}

	if (pipeline == nullptr)
	{
		Shader* shader;

		if (FrameworkConfig::api == OpenGL)
		{
			shader = new GLGUIShader();
			pipeline = new GLPipeline(shader, guiVbo);
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			shader = new VKBasicShader(window);
			pipeline = new VKPipeline(shader, guiVbo);
		}

		pipeline->CreatePipeline(window);
	}
}

void GUIElement::PreUpdate(const float delta)
{
}

void GUIElement::PreRender(RenderingEngine* renderingEngine)
{
	if (!visible)
		return;

	pipeline->BindPipeline();
	pipeline->getShader()->UpdateObjectUniforms(parent); //There are no per-shader uniforms

	guiVbo->Bind(renderingEngine->GetWindow()->GetSwapchain()->GetDrawCommandBuffer());
	guiVbo->Draw(renderingEngine->GetWindow()->GetSwapchain()->GetDrawCommandBuffer());
	guiVbo->Unbind();
	
	pipeline->UnbindPipeline();
}

GUIElement::~GUIElement()
{
	delete data;
}

void GUIElement::CleanUp()
{
	delete guiVbo;
	delete pipeline;
}