#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include "Pipeline.h"

class GraphicsPipeline
{
public:
	GraphicsPipeline(VBO* vbo);
	GraphicsPipeline() : vboIndex(-1) {}

	virtual ~GraphicsPipeline();

	static void CleanUp();

	virtual void CreatePipeline(Window* window) = 0;

	virtual void BindPipeline() const = 0;
	virtual void RenderPipeline() const = 0;
	virtual void UnbindPipeline() const = 0;

	virtual void DestroyPipeline() = 0;

	virtual void RecreatePipeline() {};

	virtual bool operator==(const Pipeline& other) = 0;

	VBO* getVbo() const { return vbos[vboIndex]; }
	void setVbo(VBO* vbo);

	bool IsWireframe() const { return wireframe; }
	bool IsBackfaceCulling() const { return backfaceCulling; }

	void SetWireframe(bool wireframe) { this->wireframe = wireframe; }
	void SetBackfaceCulling(bool backfaceCulling) { this->backfaceCulling = backfaceCulling; }
protected:
	static std::vector<VBO*> vbos;

	VBO* vbo;
	size_t vboIndex;

	bool wireframe;
	bool backfaceCulling;
};

#endif