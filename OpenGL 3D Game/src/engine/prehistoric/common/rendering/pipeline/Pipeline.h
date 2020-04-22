#ifndef PIPELINE_H
#define PIPELINE_H

#include "engine/prehistoric/common/buffer/VBO.h"
#include "engine/prehistoric/common/rendering/shaders/Shader.h"

#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/common/framework/Window.h"

class Pipeline
{
public:
	Pipeline(Shader* shader, VBO* vbo);
	Pipeline() : shaderIndex(-1), vboIndex(-1) {}

	virtual ~Pipeline();

	static void CleanUp();

	virtual void CreatePipeline(Window* window) = 0;

	virtual void BindPipeline() const = 0;
	virtual void RenderPipeline() const = 0;
	virtual void UnbindPipeline() const = 0;

	virtual void DestroyPipeline() = 0;

	virtual void RecreatePipeline() {};

	void SetShader(Shader* shader);
	void SetVbo(VBO* vbo);

	virtual bool operator==(const Pipeline& other) { return shaderIndex == other.shaderIndex && vboIndex == other.vboIndex; }

	Shader* getShader() const { return shaders[shaderIndex]; }
	VBO* getVbo() const { return vbos[vboIndex]; }

	Vector2f GetViewportStart() const { return viewportStart; }
	Vector2f GetViewportSize() const { return viewportSize; }
	Vector2u GetScissorStart() const { return scissorStart; }
	Vector2u GetScissorSize() const { return scissorSize; }

	bool IsWireframe() const { return wireframe; }
	bool IsBackfaceCulling() const { return backfaceCulling; }
	int GetSamples() const { return samples; }

	void SetViewportStart(const Vector2f& viewportStart) { this->viewportStart = viewportStart; }
	void SetViewportSize(const Vector2f& viewportSize) { this->viewportSize = viewportSize; }
	void SetScissorStart(const Vector2u& scissorStart) { this->scissorStart = scissorStart; }
	void SetScissorSize(const Vector2u& scissorSize) { this->scissorSize = scissorSize; }

	void SetWireframe(bool wireframe) { this->wireframe = wireframe; }
	void SetBackfaceCulling(bool backfaceCulling) { this->backfaceCulling = backfaceCulling; }
	int SetSamples(int samples) { this->samples = samples; }
protected:
	Shader* shader;
	VBO* vbo;

	size_t shaderIndex;
	size_t vboIndex;
	
	Vector2f viewportStart;
	Vector2f viewportSize;
	Vector2u scissorStart;
	Vector2u scissorSize;

	bool wireframe;
	bool backfaceCulling;
	int samples;

	static std::vector<Shader*> shaders;
	static std::vector<VBO*> vbos;
};

#endif