#ifndef RENDERER_H
#define RENDERER_H

#include <algorithm>

#include "Renderable.h"

class Material;

class Renderer : public Renderable
{
public:
	Renderer(MeshVBO* vbo, Shader* shader, Material* material);
	Renderer();

	virtual ~Renderer();

	void PreRender(RenderingEngine* renderingEngine) override;

	void Render(const RenderingEngine& renderingEngine) const override;
	void BatchRender(const RenderingEngine& renderingEngine) const override;

	inline Material* GetMaterial() const { return material; }
private:
	Material* material;
};

#endif