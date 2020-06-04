#ifndef RENDERING_ENGINE_H
#define RENDERING_ENGINE_H

#include "engine/platform/windows/WindowsWindow.h"
#include "engine/prehistoric/core/movement/Camera.h"
#include "engine/config/FrameworkConfig.h"

#include "engine/prehistoric/common/buffer/VBO.h"
#include "engine/prehistoric/common/rendering/pipeline/Pipeline.h"

class GameObject;
class Renderable;
class Light;

class RenderingEngine
{
public:
	RenderingEngine();
	virtual ~RenderingEngine();

	void Init() const;

	void Input();
	void Update(float delta);
	void Render(GameObject* root);

	void AddModel(Renderable* renderable);
	void AddLight(Light* light);

	inline Window* GetWindow() const { return window; }
	inline Camera* GetCamera() const { return camera; }
	inline std::vector<Light*> GetLights() const { return lights; }

	inline bool isWireframeMode() const { return wireframeMode; }

	RenderingEngine(const RenderingEngine& engine) = delete;
	RenderingEngine operator=(const RenderingEngine& engine) = delete;
private:
	std::unordered_map<Pipeline*, std::vector<Renderable*>> models;
	std::vector<Light*> lights;

	Window* window;
	Camera* camera;

	bool wireframeMode;
};

#endif