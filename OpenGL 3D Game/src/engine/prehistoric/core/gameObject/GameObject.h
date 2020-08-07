#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "engine/prehistoric/component/Component.h"

typedef void(*UpdateFunction)(GameObject*, float); //The object, and the delta time

class GameObject : public Node
{
public:
	GameObject() : updateFunction(nullptr) {}
	virtual ~GameObject();

	void PreUpdate(Engine* engine) override;
	void PreRender(RenderingEngine* renderingEngine) override;

	void setUpdateFunction(UpdateFunction func) { this->updateFunction = func; }

	GameObject* AddComponent(const std::string& name, Component* component);
	Component* GetComponent(const std::string& name) const;
protected:
	std::unordered_map<std::string, Component*> components;
	UpdateFunction updateFunction;
};

#endif