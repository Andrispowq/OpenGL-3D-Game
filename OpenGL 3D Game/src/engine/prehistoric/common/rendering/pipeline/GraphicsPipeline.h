#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include "Pipeline.h"

class GraphicsPipeline
{
public:
	GraphicsPipeline(VBO* vbo);
	GraphicsPipeline() : vbo(nullptr), wireframe(false), backfaceCulling(false), vboIndex(-1) {}

	virtual ~GraphicsPipeline();

	static void CleanUp();

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