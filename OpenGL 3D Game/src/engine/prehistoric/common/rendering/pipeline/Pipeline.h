#ifndef PIPELINE_H
#define PIPELINE_H

#include "engine/prehistoric/common/buffer/VBO.h"
#include "engine/prehistoric/common/rendering/shaders/Shader.h"

#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/common/framework/Window.h"

class Pipeline
{
public:
	Pipeline(Shader* shader);
	Pipeline() : shaderIndex(-1) {}

	virtual ~Pipeline();

	static void CleanUp();

	virtual void CreatePipeline(Window* window) = 0;

	virtual void BindPipeline() const = 0;
	virtual void RenderPipeline() const = 0;
	virtual void UnbindPipeline() const = 0;

	virtual void DestroyPipeline() = 0;

	virtual void RecreatePipeline() {};

	Shader* getShader() const { return shaders[shaderIndex]; }
	void setShader(Shader* shader);

	virtual bool operator==(const Pipeline& other) = 0;

	Vector2f GetViewportStart() const { return viewportStart; }
	Vector2f GetViewportSize() const { return viewportSize; }
	Vector2u GetScissorStart() const { return scissorStart; }
	Vector2u GetScissorSize() const { return scissorSize; }

	int GetSamples() const { return samples; }

	void SetViewportStart(const Vector2f& viewportStart) { this->viewportStart = viewportStart; }
	void SetViewportSize(const Vector2f& viewportSize) { this->viewportSize = viewportSize; }
	void SetScissorStart(const Vector2u& scissorStart) { this->scissorStart = scissorStart; }
	void SetScissorSize(const Vector2u& scissorSize) { this->scissorSize = scissorSize; }

	int SetSamples(int samples) { this->samples = samples; }
protected:
	static std::vector<Shader*> shaders;

	Shader* shader;
	size_t shaderIndex;
	
	Vector2f viewportStart;
	Vector2f viewportSize;
	Vector2u scissorStart;
	Vector2u scissorSize;

	int samples;
};

#endif