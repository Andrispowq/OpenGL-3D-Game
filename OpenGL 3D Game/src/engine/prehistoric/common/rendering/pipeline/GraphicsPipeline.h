#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include "Pipeline.h"

class GraphicsPipeline
{
public:
	GraphicsPipeline(VBO* vbo);
	GraphicsPipeline() : vbo(nullptr), backfaceCulling(false), vboIndex(-1) {}

	virtual ~GraphicsPipeline();

	static void CleanUp();

	VBO* getVbo() const { return vbos[vboIndex]; }
	void setVbo(VBO* vbo);

	bool IsBackfaceCulling() const { return backfaceCulling; }
	void SetBackfaceCulling(bool backfaceCulling) { this->backfaceCulling = backfaceCulling; }
protected:
	static std::vector<VBO*> vbos;

	VBO* vbo;
	size_t vboIndex;

	bool backfaceCulling;
};

#endif