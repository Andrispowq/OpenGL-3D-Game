#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "engine/prehistoric/component/Component.h"

class GameObject : public Node
{
public:
	GameObject() {}
	virtual ~GameObject();

	void PreUpdate(Engine* engine) override;
	void PreRender(RenderingEngine* renderingEngine) override;

	GameObject* AddComponent(const std::string& name, Component* component);
	Component* GetComponent(const std::string& name) const;
protected:
	std::unordered_map<std::string, Component*> components;
};

#endif