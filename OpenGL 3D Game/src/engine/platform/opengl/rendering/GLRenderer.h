#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "engine/prehistoric/common/rendering/Renderer.h"

#include <glew.h>

class GLRenderer : public Renderer
{
public:
	GLRenderer(Window* window) : Renderer(window) {}
	virtual ~GLRenderer() {}

	virtual void Render(Camera* camera) override;
};

#endif