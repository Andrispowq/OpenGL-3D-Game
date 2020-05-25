#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include "engine/prehistoric/component/Component.h"

#include "engine/prehistoric/common/model/Texture.h"

#include "engine/platform/opengl/buffer/GLMeshVBO.h"
#include "engine/platform/vulkan/buffer/VKMeshVBO.h"

#include "engine/platform/opengl/rendering/pipeline/GLGraphicsPipeline.h"
#include "engine/platform/vulkan/rendering/pipeline/VKGraphicsPipeline.h"

#include "engine/platform/opengl/rendering/shaders/GLShader.h"
#include "engine/platform/vulkan/rendering/shaders/VKShader.h"

class GUIElement : public Component
{
public:
	GUIElement(Window* window, Texture* texture = nullptr, void* data = nullptr, size_t dataSize = 0, bool visible = true);
	virtual ~GUIElement();

	static void CleanUp();

	virtual void PreUpdate(Engine* engine) override;
	virtual void PreRender(RenderingEngine* renderingEngine) override;

	bool isVisible() const { return visible; }
	bool hasChanged() const { return statusChanged; }

	void* getData() { return data; }
	size_t getDataSize() const { return dataSize; }

	Texture* getTexture() { return texture; }

	void setVisible(bool visible) { this->visible = visible; }

	void setData(void* data, size_t dataSize = 0) { this->data = data; this->dataSize = dataSize; }
	void setTexture(Texture* texture) { this->texture = texture; }
protected:
	bool visible;
	bool statusChanged;

	void* data;
	size_t dataSize;

	static VBO* guiVbo;
	static Pipeline* pipeline;
	Texture* texture;

	Window* window;
};

#endif