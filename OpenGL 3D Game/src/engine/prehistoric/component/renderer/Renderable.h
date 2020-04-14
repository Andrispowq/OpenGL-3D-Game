#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "engine/prehistoric/component/Component.h"
#include "engine/prehistoric/common/buffer/VBO.h"

class RenderingEngine;
class Pipeline;

class Renderable : public Component
{
public:
	Renderable(VBO* vbo, Pipeline* pipeline, Window* window);
	Renderable(Window* window);
	virtual ~Renderable();

	static void CleanUp();

	static void RecreatePipelines();

	virtual void Render(const RenderingEngine& renderingEngine) const = 0;
	virtual void BatchRender(const RenderingEngine& renderingEngine) const = 0;

	static std::vector<VBO*> GetVBOs() { return vbos; }
	static std::vector<Pipeline*> GetPipelines() { return pipelines; }

	inline size_t GetVboIndex() const { return vboIndex; }
	inline size_t GetPipelineIndex() const { return pipelineIndex; }

	inline VBO* GetVbo() const { return vbos.at(vboIndex); }
	inline Pipeline* GetShader() const { return pipelines.at(pipelineIndex); }

	Renderable(const Renderable& renderable) = delete;
	Renderable operator=(const Renderable& renderable) = delete;
protected:
	static std::vector<VBO*> vbos;
	static std::vector<Pipeline*> pipelines;

	Window* window;

	size_t vboIndex;
	size_t pipelineIndex;
};

#endif