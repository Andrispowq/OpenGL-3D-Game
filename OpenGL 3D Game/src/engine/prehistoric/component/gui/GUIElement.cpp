#include "engine/prehistoric/core/util/Includes.hpp"
#include "GUIElement.h"

#include "engine/prehistoric/core/util/ModelFabricator.h"

#include "engine/platform/opengl/rendering/shaders/gui/GLGUIShader.h"

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
			pipeline = new GLPipeline(shader);
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			shader = new VKShader(window->GetContext(), window->GetSwapchain());
			pipeline = new VKPipeline(shader);
		}

		pipeline->CreatePipeline(window, (MeshVBO*)guiVbo);
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
	pipeline->GetShader()->UpdateUniforms(parent);

	guiVbo->Bind(pipeline->GetDrawCommandBuffer(), pipeline->GetGraphicsPipeline());
	guiVbo->Draw(pipeline->GetDrawCommandBuffer());
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