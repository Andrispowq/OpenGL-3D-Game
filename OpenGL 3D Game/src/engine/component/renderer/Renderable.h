#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <vector>

#include "engine/component/Component.h"
#include "engine/core/buffer/VBO.h"

class RenderingEngine;
class Shader;

class Renderable : public Component
{
public:
	Renderable(VBO* vbo, Shader* shader);
	Renderable();
	virtual ~Renderable();

	static void CleanUp();

	virtual void Render(const RenderingEngine& renderingEngine) = 0;

	static std::vector<VBO*> GetVBOs() { return vbos; }
	static std::vector<Shader*> GetShaders() { return shaders; }

	inline size_t GetVboIndex() const { return vboIndex; }
	inline size_t GetShaderIndex() const { return shaderIndex; }

	Renderable(const Renderable& renderable) = delete;
	Renderable operator=(const Renderable& renderable) = delete;
protected:
	static std::vector<VBO*> vbos;
	static std::vector<Shader*> shaders;

	size_t vboIndex;
	size_t shaderIndex;
};

#endif