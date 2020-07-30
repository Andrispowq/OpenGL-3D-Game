#ifndef RENDERER_H
#define RENDERER_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "Renderable.h"

class Material;

class Renderer : public Renderable
{
public:
	Renderer(Pipeline* pipeline, Material* material, Window* window);
	Renderer(Window* window);

	virtual ~Renderer();

	static void CleanUp();

	void PreRender(RenderingEngine* renderingEngine) override;

	void Render(const RenderingEngine& renderingEngine) const override;
	void BatchRender(const RenderingEngine& renderingEngine) const override;

	static std::vector<Material*> getMaterials() { return materials; }

	inline size_t getMaterialIndex() const { return materialIndex; }
	inline Material* getMaterial() const { return materials.at(materialIndex); }
private:
	static std::vector<Material*> materials;

	size_t materialIndex;
};

#endif