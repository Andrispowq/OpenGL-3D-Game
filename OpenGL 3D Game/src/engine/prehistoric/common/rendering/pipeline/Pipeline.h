#ifndef PIPELINE_H
#define PIPELINE_H

#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/common/rendering/shaders/Shader.h"
#include "engine/prehistoric/common/framework/Window.h"
#include "engine/prehistoric/common/buffer/MeshVBO.h"

class Pipeline
{
public:
	Pipeline(Shader* shader) : shader(shader) {}
	Pipeline() : shader(nullptr) {}

	virtual ~Pipeline();

	virtual void CreatePipeline(Window* window, MeshVBO* vbo) = 0;

	virtual void BindPipeline() = 0;
	virtual void UnbindPipeline() const = 0;

	virtual void DestroyPipeline() = 0;

	virtual void RecreatePipeline() {};

	virtual void* GetDrawCommandBuffer() const { return nullptr; };
	virtual void* GetGraphicsPipeline() const { return nullptr; };

	void SetShader(Shader* shader) { this->shader = shader; }

	Shader* GetShader() const { return shader; }

	Vector2f GetViewportStart() const { return viewportStart; }
	Vector2f GetViewportSize() const { return viewportSize; }
	Vector2u GetScissorStart() const { return scissorStart; }
	Vector2u GetScissorSize() const { return scissorSize; }

	void SetViewportStart(const Vector2f& viewportStart) { this->viewportStart = viewportStart; }
	void SetViewportSize(const Vector2f& viewportSize) { this->viewportSize = viewportSize; }
	void SetScissorStart(const Vector2u& scissorStart) { this->scissorStart = scissorStart; }
	void SetScissorSize(const Vector2u& scissorSize) { this->scissorSize = scissorSize; }
protected:
	Shader* shader;
	
	Vector2f viewportStart;
	Vector2f viewportSize;
	Vector2u scissorStart;
	Vector2u scissorSize;

	bool wireframe;
	bool backfaceCulling;
	int samples;
};

#endif