#ifndef RENDERER_H
#define RENDERER_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "Renderable.h"

class Material;

class Renderer : public Renderable
{
public:
	Renderer(VBO* vbo, Pipeline* pipeline, Material* material, Window* window);
	Renderer(Window* window);

	virtual ~Renderer();

	static void CleanUp();

	void PreRender(RenderingEngine* renderingEngine) override;

	void Render(const RenderingEngine& renderingEngine) const override;
	void BatchRender(const RenderingEngine& renderingEngine) const override;

	static std::vector<Material*> GetMaterials() { return materials; }

	inline size_t GetMaterialIndex() const { return materialIndex; }
	inline Material* GetMaterial() const { return materials.at(materialIndex); }
private:
	static std::vector<Material*> materials;

	size_t materialIndex;
};

#endif