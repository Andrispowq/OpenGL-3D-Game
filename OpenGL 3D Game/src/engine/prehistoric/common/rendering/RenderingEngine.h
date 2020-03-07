#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include "engine/prehistoric/core/movement/Camera.h"
#include "engine/config/FrameworkConfig.h"

class GameObject;
class Renderable;

class RenderingEngine
{
public:
	RenderingEngine();
	virtual ~RenderingEngine();

	void Init() const;

	void Input(const float delta);
	void Update(const float delta);
	void Render(GameObject* root);

	void AddModel(Renderable* renderable);

	inline Window* GetWindow() const { return window; }
	inline Camera* GetCamera() const { return camera; }

	RenderingEngine(const RenderingEngine& engine) = delete;
	RenderingEngine operator=(const RenderingEngine& engine) = delete;
private:
	std::vector<Renderable*> models;

	Window* window;
	Camera* camera;
};

#endif