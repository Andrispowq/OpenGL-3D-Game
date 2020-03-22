#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "engine/prehistoric/component/Component.h"
#include "engine/prehistoric/common/buffer/MeshVBO.h"

class RenderingEngine;
class Shader;

class Renderable : public Component
{
public:
	Renderable(MeshVBO* vbo, Shader* shader, bool vboFurtherUse = false, bool shaderFurtherUse = false);
	Renderable();
	virtual ~Renderable();

	static void CleanUp();

	virtual void Render(const RenderingEngine& renderingEngine) const = 0;
	virtual void BatchRender(const RenderingEngine& renderingEngine) const = 0;

	static std::vector<MeshVBO*> GetVBOs() { return vbos; }
	static std::vector<Shader*> GetShaders() { return shaders; }

	inline size_t GetVboIndex() const { return vboIndex; }
	inline size_t GetShaderIndex() const { return shaderIndex; }

	inline MeshVBO* GetVbo() const { return vbos.at(vboIndex); }
	inline Shader* GetShader() const { return shaders.at(shaderIndex); }

	Renderable(const Renderable& renderable) = delete;
	Renderable operator=(const Renderable& renderable) = delete;
protected:
	static std::vector<MeshVBO*> vbos;
	static std::vector<Shader*> shaders;

	size_t vboIndex;
	size_t shaderIndex;
};

#endif