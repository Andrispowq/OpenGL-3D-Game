#ifndef RENDERER_H
#define RENDERER_H

#include <algorithm>

#include "Renderable.h"

class Renderer : public Renderable
{
public:
	Renderer(VBO* vbo, Shader* shader);
	Renderer();

	virtual ~Renderer();

	void PreRender(RenderingEngine* renderingEngine) override;

	void Render(const RenderingEngine& renderingEngine) override;
private:
};

#endif