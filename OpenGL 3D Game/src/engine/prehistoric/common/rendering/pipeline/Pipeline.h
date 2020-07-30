#ifndef PIPELINE_H
#define PIPELINE_H

#include "engine/prehistoric/common/buffer/VertexBuffer.h"
#include "engine/prehistoric/common/rendering/shaders/Shader.h"

#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/common/framework/Window.h"

#include "engine/prehistoric/resources/AssetManager.h"

class Pipeline
{
public:
	Pipeline(AssetManager* manager, size_t shaderID);
	Pipeline() : samples(0), shaderID(-1) {}

	virtual ~Pipeline();

	virtual void CreatePipeline(Window* window) = 0;

	virtual void BindPipeline() const = 0;
	virtual void RenderPipeline() const = 0;
	virtual void UnbindPipeline() const = 0;

	virtual void DestroyPipeline() = 0;

	virtual void RecreatePipeline() {};

	Shader* getShader() const { return assetManager->getShader(shaderID); }

	size_t getShaderID() const { return shaderID; }
	void setShaderID(size_t shaderID);

	Vector2f getViewportStart() const { return viewportStart; }
	Vector2f getViewportSize() const { return viewportSize; }
	Vector2u getScissorStart() const { return scissorStart; }
	Vector2u getScissorSize() const { return scissorSize; }

	int getSamples() const { return samples; }

	void setViewportStart(const Vector2f& viewportStart) { this->viewportStart = viewportStart; }
	void setViewportSize(const Vector2f& viewportSize) { this->viewportSize = viewportSize; }
	void setScissorStart(const Vector2u& scissorStart) { this->scissorStart = scissorStart; }
	void setScissorSize(const Vector2u& scissorSize) { this->scissorSize = scissorSize; }

	void setSamples(int samples) { this->samples = samples; }
public:
	AssetManager* assetManager;

	size_t shaderID;
	
	Vector2f viewportStart;
	Vector2f viewportSize;
	Vector2u scissorStart;
	Vector2u scissorSize;

	int samples;
};

#endif