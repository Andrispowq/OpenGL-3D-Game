#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "engine/prehistoric/component/Component.h"
#include "engine/prehistoric/common/buffer/MeshVBO.h"

class RenderingEngine;
class Pipeline;

class Renderable : public Component
{
public:
	Renderable(MeshVBO* vbo, Pipeline* shader, bool vboFurtherUse = false, bool shaderFurtherUse = false);
	Renderable();
	virtual ~Renderable();

	static void CleanUp();

	virtual void Render(const RenderingEngine& renderingEngine) const = 0;
	virtual void BatchRender(const RenderingEngine& renderingEngine) const = 0;

	static std::vector<MeshVBO*> GetVBOs() { return vbos; }
	static std::vector<Pipeline*> GetPipelines() { return pipelines; }

	inline size_t GetVboIndex() const { return vboIndex; }
	inline size_t GetPipelineIndex() const { return pipelineIndex; }

	inline MeshVBO* GetVbo() const { return vbos.at(vboIndex); }
	inline Pipeline* GetShader() const { return pipelines.at(pipelineIndex); }

	Renderable(const Renderable& renderable) = delete;
	Renderable operator=(const Renderable& renderable) = delete;
protected:
	static std::vector<MeshVBO*> vbos;
	static std::vector<Pipeline*> pipelines;

	size_t vboIndex;
	size_t pipelineIndex;
};

#endif