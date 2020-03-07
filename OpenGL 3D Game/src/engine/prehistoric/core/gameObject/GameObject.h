#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <unordered_map>

#include "engine/prehistoric/component/Component.h"

class GameObject : public Node
{
public:
	GameObject() {}
	virtual ~GameObject();

	void PreInput(const float delta) override;
	void PreUpdate(const float delta) override;
	void PreRender(RenderingEngine* renderingEngine) override;

	GameObject* AddComponent(const std::string& name, Component* component);
	Component* GetComponent(const std::string& name) const;
private:
	std::unordered_map<std::string, Component*> components;
};

#endif