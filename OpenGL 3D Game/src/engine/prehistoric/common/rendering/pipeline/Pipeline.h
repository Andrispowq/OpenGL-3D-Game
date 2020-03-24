#ifndef PIPELINE_H
#define PIPELINE_H

#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/common/rendering/shaders/Shader.h"

class Pipeline
{
public:
	Pipeline(Shader* shader) : shader(shader) {}
	Pipeline() : shader(nullptr) {}

	virtual ~Pipeline();

	virtual void CreatePipeline() = 0;

	virtual void BindPipeline() const = 0;
	virtual void UnbindPipeline() const = 0;

	virtual void DestroyPipeline() = 0;

	void SetShader(Shader* shader) { this->shader = shader; }

	Shader* GetShader() const { return shader; }

	Vector2f GetViewportStart() const { return viewportStart; }
	Vector2f GetViewportSize() const { return viewportSize; }
	Vector2f GetScissorStart() const { return scissorStart; }
	Vector2f GetScissorSize() const { return scissorSize; }

	void SetViewportStart(const Vector2f& viewportStart) { this->viewportStart = viewportStart; }
	void SetViewportSize(const Vector2f& viewportSize) { this->viewportSize = viewportSize; }
	void SetScissorStart(const Vector2f& scissorStart) { this->scissorStart = scissorStart; }
	void SetScissorSize(const Vector2f& scissorSize) { this->scissorSize = scissorSize; }
protected:
	Shader* shader;

	Vector2f viewportStart;
	Vector2f viewportSize;
	Vector2f scissorStart;
	Vector2f scissorSize;

	bool wireframe;
	bool backfaceCulling;
	int samples;
};

#endif