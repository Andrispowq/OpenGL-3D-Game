#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "RenderableComponent.h"

class Material;

class RendererComponent : public RenderableComponent
{
public:
	RendererComponent(Pipeline* pipeline, Material* material, Window* window);
	RendererComponent(Window* window);

	virtual ~RendererComponent();

	static void CleanUp();

	void PreRender(Renderer* renderer) override;

	void Render(Renderer* renderer) const override;
	void BatchRender() const override;

	static std::vector<Material*> getMaterials() { return materials; }

	inline size_t getMaterialIndex() const { return materialIndex; }
	inline Material* getMaterial() const { return materials.at(materialIndex); }
private:
	static std::vector<Material*> materials;

	size_t materialIndex;
};

#endif