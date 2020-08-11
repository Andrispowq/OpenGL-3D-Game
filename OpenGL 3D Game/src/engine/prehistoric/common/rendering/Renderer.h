#ifndef RENDERER_H
#define RENDERER_H

#include "engine/prehistoric/common/framework/Window.h"
#include "engine/prehistoric/core/movement/Camera.h"

#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"

class RenderableComponent;

class Renderer
{
public:
	Renderer(Window* window, Camera* camera) : window(window), camera(camera) {}
	virtual ~Renderer() {}

	void PrepareRendering();
	void EndRendering();

	virtual void Render() = 0;

	void AddModel(RenderableComponent* renderable);
	void AddLight(Light* light);

	Window* getWindow() const { return window; }
	Camera* getCamera() const { return camera; }

	inline std::vector<Light*> getLights() const { return lights; }
	inline Light* getSun() const { return sun; }

	inline bool isWireframeMode() const { return wireframeMode; }

protected:
	Window* window;
	Camera* camera;

	//The rendering context
	std::unordered_map<Pipeline*, std::vector<RenderableComponent*>> models_3d;
	std::unordered_map<Pipeline*, std::vector<RenderableComponent*>> models_transparency;
	std::unordered_map<Pipeline*, std::vector<RenderableComponent*>> models_2d;

	std::vector<Light*> lights;
	Light* sun = nullptr; //Not owned

	bool wireframeMode;
};

#endif