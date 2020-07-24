#include "engine/prehistoric/core/util/Includes.hpp"
#include "GUIElement.h"

#include "engine/prehistoric/core/util/ModelFabricator.h"

#include "engine/platform/opengl/rendering/shaders/gui/GLGUIShader.h"
#include "engine/platform/vulkan/rendering/shaders/basic/VKBasicShader.h"

#include "engine/prehistoric/core/Engine.h"

size_t GUIElement::vboID = -1;
Pipeline* GUIElement::pipeline = nullptr;

GUIElement::GUIElement(Window* window, AssetManager* manager, Texture* texture, void* data, size_t dataSize, bool visible)
	: type(GUIType::Element)
{
	this->texture = texture;
	this->data = data;
	this->dataSize = dataSize;
	this->visible = visible;

	this->window = window;

	if (vboID == -1)
	{
		vboID = manager->addVertexBuffer(ModelFabricator::CreateQuad(window));
		manager->getVertexBuffer(vboID)->SetFrontFace(FrontFace::CLOCKWISE);
	}

	if (pipeline == nullptr)
	{
		size_t shaderID;

		if (FrameworkConfig::api == OpenGL)
		{
			shaderID = manager->addShader(new GLGUIShader());
			pipeline = new GLGraphicsPipeline(manager, shaderID, vboID);
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			shaderID = manager->addShader(new VKBasicShader(window));
			pipeline = new VKGraphicsPipeline(manager, shaderID, vboID);
		}

		pipeline->CreatePipeline(window);
	}

	Renderer* renderer = new Renderer(pipeline, nullptr, window);
	renderer->setPriority(RenderPriority::_2D);

	AddComponent(RENDERER_COMPONENT, renderer);
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

bool GUIElement::inside(Vector2f cursor)
{
	cursor.y = window->GetHeight() - cursor.y;
	cursor /= { (float)window->GetWidth(), (float)window->GetHeight() };
	cursor *= 2;
	cursor -= 1;

	Vector2f pos = worldTransform->GetPosition().xy();
	Vector2f scale = worldTransform->GetScaling().xy();

	Vector2f start = pos - scale;
	Vector2f end = pos + scale;

	if (start <= cursor && end >= cursor)
	{
		return true;
	}

	return false;
}

GUIElement::~GUIElement()
{
	//this data is most likely getting a pointer to a stack variable, so it is not ideal to delete it
	//delete data;
}

void GUIElement::CleanUp()
{
	delete pipeline;
}